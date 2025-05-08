#ifndef HTABLE_H
#define HTABLE_H

#include<stdbool.h>
#include<ctype.h>


typedef struct HTNode{
    void *key;
    void *value;
    struct HTNode *next;
}HTNode;

typedef struct HTLinkedlist{
    HTNode *head;
}HTLinkedlist;

typedef struct HTable{
    int size; 
    int num_entries;
    double load_factor;
    HTLinkedlist *slots[];
}HTable;


void initHT(HTable **t,int s);
void resizeHT(HTable **t,int new_size);
void ht_print(HTable *t);
bool ht_gettype(char *k);
int ht_code(int table_size,void *k);
void ht_set(HTable **t,void *nd);
void *ht_get(HTable **t,void *k);
void ht_del(HTable **t,void *k);


#endif