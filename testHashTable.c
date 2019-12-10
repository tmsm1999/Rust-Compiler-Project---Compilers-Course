#include <stdio.h>
#include "hash.h"
#include "list.h"

int main() {

    HASH_TABLE myHashTable = init_table();
    insert(myHashTable, "Tomas");
    printf("%s\n", lookup(myHashTable, "Tomas") -> variable);
    printf("%s\n", lookup(myHashTable, "Fabio") -> variable);





    return 0;
}
