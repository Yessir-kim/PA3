#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/libstemmer.h"

int 
main () 
{
	struct sb_stemmer * stemmer ;
	char buf[1024] ;
	int i=0;
	char stor[1024][1024];
	stemmer = sb_stemmer_new("english", 0x0) ;
	FILE *f = fopen("stopwordlist.txt", "r");

	do {
		const char * s ;

		fscanf(f, "%s\n", buf) ;
		//fgets(buf, sizeof(buf), f);
		s = sb_stemmer_stem(stemmer, buf, strlen(buf)) ;
		printf("%s\n", s) ;
		    if(strcmp(stor[i-1],s) == 0){
			continue;
		    }
		strcpy(stor[i],s);
		i++;
	} while ( !feof( f)) ;

	sb_stemmer_delete(stemmer) ;
	fclose(f);
	char ss[1024];
	FILE *fp = fopen("stopword.txt", "w");
	for(int j=0; j<i; j++){
	    strcpy(ss, stor[j]);
	    fprintf(fp, "%s\n", stor[j]);
	}
}
