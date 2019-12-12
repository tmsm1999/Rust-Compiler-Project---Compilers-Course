#include <stdio.h>
#include "hash.h"
#include "list.h"

int main() {

    HASH_TABLE myHashTable = init_table();
    insert(myHashTable, "Tomas");
    insert(myHashTable, "Fabio");
    insert(myHashTable, "Ze");
    insert(myHashTable, "Vasco");
    insert(myHashTable, "Ruben");
    //printf("%s\n", lookup(myHashTable, "Tomas") -> variable);
    //printf("%s\n", lookup(myHashTable, "Fabio") -> variable);

    printf(".data\n");
    for(int i = 0; i < HASH_SIZE; i++) {
    	if(myHashTable -> array[i] != NULL) {
    		LIST_HT cur = myHashTable -> array[i];
    		while(cur != NULL) {
    			printf("%s\n", cur -> variable);
    			cur = cur -> next;
    		}
    	} 
    }



    return 0;
}
