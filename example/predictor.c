#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glib.h>
#include "../include/libstemmer.h"

void
print_counter (gpointer key, gpointer value, gpointer userdata)
{
    char * t = key ;
    char * d = value ;
    //if(*d > 100 && *d < 1000)
    printf("(%s, %s)\n", t, d) ;
}


int isStringnumber(const char *c)
{
    int size = strlen(c) ;
    if(size == 0) return 1 ;
    for(int i = 0 ; i < size ; i++)
        if(c[i] >= '0' && c[i] <= '9') return 1 ; // number case
    return 0 ;
}

char * checkString(char *c)
{
    int size = strlen(c) ;
    int tempSize = 0 ;
    int i = 0 ;
    int count = 0 ;
    while(size > 0)
    {
        if(c[i] >= 'a' && c[i] <= 'z')
            count++ ;
        i++ ;
        size--;
    }
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
            c[i] = c[i] + 32 ;
        }
    }
}

int isStopword(const char *c){
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
}

int main(int argc, char** argv)
{
    
    char * word;
    char * line2 = 0x0 ;
    size_t n = 0 ;
    int num;
    GHashTable* counter = g_hash_table_new(g_str_hash,g_str_equal);
    GHashTable* Ncounter = g_hash_table_new(g_str_hash,g_str_equal);
    
    FILE * f = fopen("model.csv","r");
    
    while (getline(&line2, &n, f) >= 0) {
        char * t ;
        num=0;
        char * _line2 = line2 ;

        for (t = strtok(line2, ",") ; t != 0x0 ; t = strtok(0x0, ",")) {
            num++;
            if(num==1) word=t;
            else{
                if(num==2) g_hash_table_insert(counter,strdup(word),strdup(t));
                if(num==3){
                    for(int i=0;i<=strlen(t);i++)
                        if(t[i]=='\n')t[i]='\0';
                            g_hash_table_insert(Ncounter,strdup(word),strdup(t));
                }
            }
        }
        free(_line2) ;
        line2 = 0x0 ;
    }
    
    //***************************************************************************
    struct sb_stemmer * stemmer ;
    stemmer = sb_stemmer_new("english", 0x0) ;
    
    // FILE * f1 = fopen("../data/test.negative.csv", "r");
    // FILE * f2 = fopen("../data/test.non-negative.csv", "r");

    FILE *fp = fopen(argv[1], "r");

    char * line = 0x0 ;

    int count = 0;
    
    while (getline(&line, &n, fp) >= 0) {
      char * t ;
      char * _line = line ;
      const char * s ;
      int i, size;

      printf("Your sentence : %s \n", line);

        GHashTable * sentenceHash = g_hash_table_new(g_str_hash, g_str_equal) ;
      for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t"))
      {
          int * d ;
          convertUpper(t) ;
          t = checkString(t) ;
          size = strlen(t) ;
          
          if(size == 1) continue ; // delete alphabet form

          for(i = 0 ; i < size ; i++)
          {
              if(t[i] < 0) break ;
              if(!(t[i] >=97 && t[i] <= 122)) break ;
          }
          
          if(i != size) continue;

          s = sb_stemmer_stem(stemmer, t, size) ;
          
          if(isStringnumber(s) == 0 && isStopword(s) == 0)
          {
              d = g_hash_table_lookup(sentenceHash, s) ;
              if (d == NULL) { // new word -> memory allocation
                  d = malloc(sizeof(int)) ;
                  *d = 0 ;
                  g_hash_table_insert(sentenceHash, strdup(s), d) ;
              }
          }

      }

        GList * sentecelist = NULL ;
        sentecelist = g_hash_table_get_keys(sentenceHash) ;
        
        // printf("**********start***********");
        // g_hash_table_foreach(sentenceHash, print_counter, 0x0) ;
        // printf("**********end***********");

        char * neg;
        char * nonneg;
        double nnprob, nprob;
        double nn = 0,n = 0;
        double log_n = 0,log_nn = 0;

        for(GList *Ltemp = sentecelist ; Ltemp != NULL ; Ltemp = Ltemp->next)
        {
            //printf("!!!%s",key);
            /*
            int *VNnegative = (int*)g_hash_table_lookup(sentenceHash,Ltemp->data) ;
            printf("%d",*VNnegative);
            */
            neg = g_hash_table_lookup(counter,Ltemp->data);
            nonneg = g_hash_table_lookup(Ncounter,Ltemp->data);
            
            if(neg != NULL)
            {
                nprob = atof(neg);
                log_n += log(nprob);
            }
            if(nonneg != NULL)
            {
                nnprob = atof(nonneg);
                log_nn += log(nnprob);
            }

        }
    
        double prob = exp(log_n) / (exp(log_n) + exp(log_nn));
        
        // printf("probability : %f\n",prob);

        if(prob > 0.65){
            printf("The sentence is Negative\n");

        }
        else{
             printf("The sentence is Non negative\n");
        }
    }
}
