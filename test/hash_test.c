#include <stddef.h>
#include "../include/zmalloc.h"
#include "../include/sds.h"
#include "../include/dict.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define N 1024
void assert_equal(int test,int expect){
    if(test!=expect){
        fprintf(stderr,"test: %d  expect:%d\n",test,expect);
        exit(1);
    }
}
void dictCharDestructor(void *privdata, void *val)
{
    DICT_NOTUSED(privdata);
    free(val);
}
/*
 * The method to compare two sds keys
 */
int dictCharKeyCompare(void *privdata, const void *key1,
        const void *key2)
{
    int l1,l2;
    DICT_NOTUSED(privdata);

    l1 = strlen((char*)key1);
    l2 = strlen((char*)key2);
    if (l1 != l2) return 0;
    return memcmp(key1, key2, l1) == 0;
}
/*
 * The hash function of the key
 */
unsigned int dictCharHash(const void *key) {
    return dictGenHashFunction((char*)key, strlen((char*)key));
}

/*
 * sds pair dict
 */
dictType sdsDictType = {
    dictCharHash,                /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictCharKeyCompare,          /* key compare */
    dictCharDestructor,          /* key destructor */
    dictCharDestructor           /* val destructor */
};
void sds_dict_test(){
    dict *sds_dict;
    int i;
    char* k;
    char* v;
    char  testk[100];
    char  testv[100];
    char buffer[100];
    char* tmp;
    int cnt;
    sds_dict = dictCreate(&sdsDictType,NULL);
    //store
    for(i=0;i<N;++i){
        snprintf(buffer,100,"%d",i);
        k=malloc((strlen(buffer)+1)*sizeof(char));
        k=strcpy(k,buffer);
        snprintf(buffer,100,"%d",N-i);
        v=malloc((strlen(buffer)+1)*sizeof(char));
        v=strcpy(v,buffer);
        dictAdd(sds_dict,k,v);
    }
    printf("Finish Storing\n");
    //lookup
    for(i=0;i<N;++i){
        snprintf(testk,100,"%d",i);
        snprintf(testv,100,"%d",N-i);
        tmp = (char*)dictFetchValue(sds_dict,testk);
        if(!tmp){
            fprintf(stderr,"should be found out\n");
            exit(1);
        }
        assert_equal(strcmp(testv,tmp),0);
    }
    printf("Finish looking up\n");
    //delete odd numbers
    cnt = 0;
    for(i=0;i<N;++i){
        snprintf(testk,100,"%d",i);
        if(i%2 == 0)
            dictDelete(sds_dict,testk);
    }
    printf("Finish deleting\n");
    for(i=0;i<N;++i){
        snprintf(testk,100,"%d",i);
        if(!dictFetchValue(sds_dict,testk))
            ++cnt;
    }
    printf("Finish counting\n");
    assert_equal(cnt,512);
    dictRelease(sds_dict);
}
int main(int argc, char** argv){
    sds_dict_test();
    return 0;
}
