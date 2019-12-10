#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "listHashTable.h"

// Create a new hash table list
LIST_HT newList_HT(char* var, LIST_HT l) {
  LIST_HT new = malloc(sizeof(struct list_ht));
  new->variable = var;
  new->next = l;
  return new;
}
