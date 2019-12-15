#ifndef __listHashTable_h__
#define __listHashTable_h__

#define bool int

// Hash table list structure
typedef struct list_ht {
  char* variable;
  struct list_ht* next;
} *LIST_HT;

LIST_HT newList_HT(char* var, LIST_HT l);

#endif