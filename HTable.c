#include "Htable.h"

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>


#define SCALED_GOLDEN_RATIO 0.61803398875

void initHT(HTable **t,int s){

    *t = (HTable*)(malloc(sizeof(HTable) + s*sizeof(HTLinkedlist)));
   
    int i;
    for(i=0;i<s;i++){
        (*t)->slots[i] = (HTLinkedlist*)(malloc(sizeof(HTLinkedlist)));
        (*t)->slots[i]->head = NULL;
    }
     
    (*t)->size = s;
    (*t)->num_entries = 0;
    (*t)->load_factor = 0;
    
}


void resizeHT(HTable **t,int new_size){
    
    HTable *new_table = (HTable*)(malloc(sizeof(HTable) + new_size*sizeof(HTLinkedlist)));

    int i;
    for(i=0;i<new_size;i++){
        if(i < (*t)->size)
        new_table->slots[i] = (*t)->slots[i];
        else{
            new_table->slots[i] = (HTLinkedlist*)(malloc(sizeof(HTLinkedlist)));
            new_table->slots[i]->head = NULL;
        }
    }

    new_table->num_entries = (*t)->num_entries;
    new_table->load_factor = (*t)->load_factor;
    new_table->size = new_size;

    *t = new_table;
}


void ht_print(HTable *t){
    if(t->num_entries == 0)
    printf("The hashtable is empty");
    else{
        int i;
        for(i=0;i<t->size;i++){
            if(t->slots[i]->head == NULL)
            printf("Slot %d has no entries \n \n",i);
            else{
                printf("Slot %d has the following entries: \n",i);
                HTNode *aux = t->slots[i]->head;
                while(aux != NULL){
                    printf("Key: %s <-> Value: %s || ",(char*)aux->key,(char*)aux->value);
                    aux = aux->next;
                }

                printf("\n \n");
            }
        }
    }
}

//Checks whether input is string or integer. If it is an integer it returns true else it returns false.
bool ht_gettype(char *k){

    int i = 0;
    bool b = true;
    while(i < strlen(k) && b){
        if( isdigit(k[i]) ) 
        i++;
        else b = false;
    }

    return b;
}

int ht_code(HTable *t,void *k){
    int hcode = 0;
    int total = 0;

    if( ht_gettype(k) ){
        int i;
        for(i = 0;i<strlen(k);i++)
        total = total*10 + ( ((char*)k)[i] - '0');

        total = ((total << 4)^total)*SCALED_GOLDEN_RATIO;    
        hcode = total%t->size;
    }else{
        int x;
        for(x=0;x<strlen(k);x++)
        total = total + (int)(((char*)k)[x]);
        
        total = ((total << 4)^total)*SCALED_GOLDEN_RATIO;    
        hcode = total%t->size;

    }
    
    return hcode;
}

void ht_set(HTable *t,void *k,void *v){

    int code = ht_code(t,k);
    HTNode *nd = (HTNode*)(malloc(sizeof(HTNode)));
    nd->key = k;
    nd->value = v;
    nd->next = NULL;

    if(t->load_factor >= 75)
    resizeHT(&t,2*(t->size)+1);

    if(t->slots[code]->head == NULL){
        t->slots[code]->head = nd;
        t->num_entries++;
        t->load_factor = (t->num_entries/t->size)*100; 
    }else{
        HTNode *aux = t->slots[code]->head;
        bool b = (aux->key == k);

        while(aux->next != NULL && !b){
            aux = aux->next;
            if(aux->key == k)
            b = true;
        }
        
        if(b)
        printf("This entry already exists");
        else{
            aux->next = nd;
            t->num_entries++;
            t->load_factor = (t->num_entries/t->size)*100;
        }
        
    }
        
}

void *ht_get(HTable *t,void *k){

    int code = ht_code(t,k);

    HTNode *aux = t->slots[code]->head;
    bool b = false;
    while(aux != NULL && !b){
        if(strcmp(aux->key,k)==0)
        b = true;
        else aux = aux->next;
    }

    if(b)
    return aux->value;
    else return NULL;
    
}

void ht_del(HTable *t,void *k){
    int code = ht_code(t,k);
    bool b = false;

    if(code < t->size)
    {
        HTNode *aux = t->slots[code]->head;
        HTNode *prev = NULL;
        while(aux != NULL && !b){
            if(strcmp(aux->key,k) != 0){
                prev = aux;
                aux = aux->next;
            }else b = true;
        }
    
    if(!b)
    printf("This entry does not exist");
    else{
        if(prev == NULL)
        t->slots[code]->head = t->slots[code]->head->next;
        else prev->next = aux->next;

        t->num_entries--;
    }
   }
}


void main(){

    HTable *tb;


    
    printf("Give me the size of your hashtable \n");
    int sz;
    scanf("%d",&sz);
    initHT(&tb,sz);
    printf("\n");

    int entries;
    printf("Give me the number of entries you would like to insert \n");
    scanf("%d",&entries);

    int i = 0;
    while(i < entries){
        char *k_e = (char*)(malloc(sizeof(char)));
        char *v_e = (char*)(malloc(1000*sizeof(char)));

        printf("Give me the key and value of entry %d \n",i);
        scanf("%s",k_e);
        scanf("%s",v_e);
        ht_set(tb,k_e,v_e);
        i++;
        printf("\n");
    }

    ht_print(tb);

    printf("\n");


    
    char *t = (char*)(malloc(1000*sizeof(char)));
    while(true){
        printf("Give me the entry-key you are looking for: \n");
        scanf("%s",t);
        char *vl = ht_get(tb,t);
        
        if(vl == NULL)
        printf("This entry does not exist \n");
        else printf("The value you are looking for is %s :",vl);

        printf("\n");
    }
        

}





/*
    char *ky = (char*)(malloc(1000*sizeof(char)));
    while(1){
        int ent = tb->num_entries;
        printf("Give me the entry-key that you would like to delete: \n");
        scanf("%s",ky);
        ht_del(tb,ky);

        if(tb->num_entries < ent){
            printf("Your new table is \n \n");
            ht_print(tb);
        }
        printf("\n");
    }    
*/