#include <stdio.h>
#include "listHashTable.h"


#define HASH_SIZE  500 // Prime number
#define MULTIPLIER 32       // 31, 32 and 33 are good to avoid collisions

typedef struct hashtable {
    unsigned int size;
    LIST_HT *array;
} *HASH_TABLE;


unsigned int hash(char *var);

LIST_HT lookup(HASH_TABLE ht, char *var);

void insert(HASH_TABLE ht, char *var);

HASH_TABLE init_table();
