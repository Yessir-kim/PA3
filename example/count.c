#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/libstemmer.h"

typedef struct
{
	char key ;
	double Neg ;
	double nonNeg ;
} TRIPLE ; 
	
void 
print_counter (gpointer key, gpointer value, gpointer userdata) 
{
	char * t = key ;
	int * d = value ;

	if(*d > 100 && *d < 1000)  
		printf("(%s, %d)\n", t, *d) ;
}

int
isStringnumber(const char *c)
{

	int size = strlen(c) ;

	// If size is 0, then we don't have to  check this case
	if(size == 0) return 1 ;
	
	// number check line	
	for(int i = 0 ; i < size ; i++) 
		if(c[i] >= '0' && c[i] <= '9') return 1 ; // number case

	return 0 ; // word case
}

char*
checkString(char *c)
{	
	int size = strlen(c) ;
	int tempSize = 0 ;
	int i = 0 ;
	int count = 0 ;

	while(size > 0)
	{
		if(c[i] >= 'a' && c[i] <= 'z')
                {
			count++ ;
             	}
		i++ ;
		size-- ;
	}
	
	//printf("%d ",tempSize) ;
	
	char *store = (char*)malloc(sizeof(char) * count) ;
	
	i = 0 ;
	tempSize = 0 ;

	while(count > 0)
        {
                if(c[i] >= 'a' && c[i] <= 'z')
                {
                        store[tempSize++] = c[i] ;
        		count-- ;        
		}
                i++ ;
        }
	/* DEBUG LINE
	for(int j = 0; j < tempSize; j++)
		printf("%c ",store[j]) ; 
	printf("\n") ;
	*/
	return store ;	
}

void
convertUpper(char *c)
{
	int size = strlen(c) ;
	
	for(int i = 0 ; i < size ; i++)
	{
		if(c[i] >= 65 && c[i] <= 90)
		{	
			//printf("%c ",c[i]) ;
			c[i] = c[i] + 32 ;
			//printf("%c",c[i]) ;
		}
	}
	//printf("%s\n",c) ;
}

int
isStopword(const char *c){
	char buf[1024];
	int count = 0;
    	FILE* fptr = fopen("stopword.txt", "r");
	while(!feof(fptr)){
	    fscanf(fptr, "%s\n", buf) ;
	    if( strcmp(buf, c) == 0 ){
		fclose(fptr);
		return 1;
	    }
	    if( buf[0] > c[0] )
		break;
	    else if( buf[0] == c[0] ){
		if(buf[1] > c[1]){
			break;
		}
	    }
	}
	fclose(fptr);
	return 0;
//    return 0;
}



int 
main () 
{

	TRIPLE store ; // structure declare

	struct sb_stemmer * stemmer ;

        stemmer = sb_stemmer_new("english", 0x0) ;
	
	FILE * f = fopen("../data/train.negative.csv", "r") ;
	// create two hash table that each negative and non_negative 
	GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ; // negtive 

	GHashTable * Ncounter = g_hash_table_new(g_str_hash, g_str_equal) ; // non_negative

	
	char * line = 0x0 ;
	size_t r ; 
	size_t n = 0 ;

	while (getline(&line, &n, f) >= 0) {
		char * t ;
		char * _line = line ;
		const char * s;
		// Tokeniztion part		
		for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {
			int * d ;
			int i, size ;

			convertUpper(t) ;
	
			t = checkString(t) ;
				
			size = strlen(t) ;
			
			if(size == 1) continue ; // delete alphabet form			

			for(i = 0 ; i < size ; i++)
			{
				if(t[i] < 0) break ;
				if(!(t[i] >=97 && t[i] <= 122)) break ;
			}

			if(i != size) continue ;
			// Normalization part
			s = sb_stemmer_stem(stemmer, t, size) ;
			
			/* create array and remove word that is, are, am etc..
 		      	g_hash_table_remove(counter,"is") ;
       			*/
	//		fclose(f);

			if(isStringnumber(s) == 0 && isStopword(s) == 0)
			{			
				d = g_hash_table_lookup(counter, s) ;
				if (d == NULL) { // new word -> memory allocation
					d = malloc(sizeof(int)) ;
					*d = 1 ;
					g_hash_table_insert(counter, strdup(s), d) ;
				}
				else {
					*d = *d + 1 ; // exist word -> count +1
				}
			}
		}
		free(_line) ;
		line = 0x0 ;
	}

	g_hash_table_foreach(counter, print_counter, 0x0) ;
	
	//printf("but: %d\n", *((int *) g_hash_table_lookup(counter, "is"))) ;

	fclose(f) ;

	FILE * fp = fopen("../data/train.non-negative.csv", "r") ;

        line = 0x0 ;
        n = 0 ;

        while (getline(&line, &n, fp) >= 0) {
                char * t ;
                char * _line = line ;
		const char * s ;
                // Tokeniztion part
                for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {
                        int * d ;
                        int i, size ;

                        convertUpper(t) ;

                        t = checkString(t) ;

                        size = strlen(t) ;
				
			if(size == 1) continue ; // delete alphabet form

                        for(i = 0 ; i < size ; i++)
                        {
                                if(t[i] < 0) break ;
                                if(!(t[i] >=97 && t[i] <= 122)) break ;
                        }

                        if(i != size) continue ;
			
			s = sb_stemmer_stem(stemmer, t, size) ;
		
//			fclose(fp);

                        if(isStringnumber(s) == 0 && isStopword(s) == 0)
                        {
                                d = g_hash_table_lookup(Ncounter, s) ;
                                if (d == NULL) { // new word -> memory allocation
                                        d = malloc(sizeof(int)) ;
                                        *d = 1 ;
                                        g_hash_table_insert(Ncounter, strdup(s), d) ;
                               	}
                               	else {
                                       *d = *d + 1 ; // exist word -> count +1
                               	}
                       	}
               	}
               	free(_line) ;
               	line = 0x0 ;
       	}
       	printf("\n") ;
       	g_hash_table_foreach(Ncounter, print_counter, 0x0) ;

       	//printf("but: %d\n", *((int *) g_hash_table_lookup(Ncounter, "servic"))) ;
       	fclose(fp) ;
	sb_stemmer_delete(stemmer) ;
	
	// probablity cal part
	
}
