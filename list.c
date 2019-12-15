#include <stdio.h>
#include <stdlib.h>
#include "list.h"


// Create a new list
NODE newList(char* var, NODE l) {
  NODE new = malloc(sizeof(struct node));
  new->variable = var;
  new->next = l;
  return new;
}


// Add an instruction to the end of the list
void addLast(char* var, NODE *l) {
  if (*l == NULL) *l = newList(var, NULL);
  else {
    NODE cur = *l;
    while (cur != NULL && cur->next != NULL) cur = cur->next;
    NODE end = malloc(sizeof(struct node));
    end->variable = var;
    end->next = NULL;
    cur->next = end;
  }
}


// Print the list
void printList(NODE l) {
  if (l == NULL) {
    printf("\n"); return;
  }
  printf("%s", l->variable);
  if (l->next != NULL) printf(" ");
  printList(l->next);
}