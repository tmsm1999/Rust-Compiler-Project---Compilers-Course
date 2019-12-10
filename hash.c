#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"


// Get the integer hash of a string
unsigned int hash(char *var) {
    unsigned char *p;
    unsigned int h = 0;
    for (p = (unsigned char*) var; *p != '\0'; p++)
        h = MULTIPLIER * h + *p;

    return h % HASH_SIZE;
}


// Get the node mapped by the hash table
LIST_HT lookup(HASH_TABLE ht, char *var) {
    unsigned int index = hash(var);
    LIST_HT cur = ht->array[index];
    while (cur != NULL && strcmp(cur->variable, var) != 0)
        cur = cur->next;

    return cur;
}


// Insert a (key, value) pair into the hash table
void insert(HASH_TABLE ht, char *var) {
    // If it already exists, replace it. Otherwise, add it to the head of the list
    LIST_HT l = lookup(ht, var);
    if (l == NULL) {
        unsigned int index = hash(var);
        ht->array[index] = newList_HT(var, ht->array[index]);
    }
}


// Create a new hash table
HASH_TABLE init_table() {
    HASH_TABLE ht = (HASH_TABLE) malloc(sizeof(struct hashtable));
    ht->size = HASH_SIZE;
    ht->array = (LIST_HT*) malloc(ht->size*sizeof(struct list_ht));
    for (int i = 0; i < ht->size; i++)
        ht->array[i] = NULL;

    return ht;
}
