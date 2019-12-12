#include <stdio.h>

void printMIPS(InstrList* intermCode) {
    Instr* instr = intermCode -> first;

    printf(".data\n"); //begining of data section
    for(int i = 0; i < HASH_SIZE; i++) {
    	if(variables -> array[i] != NULL) {
    		LIST_HT cur = variables -> array[i];
    		while(cur != NULL) {
    			printf("%s: .space 4\n", cur -> variable);
    		}
    	} 
    }
    //end of data section


    //begining of text section
    printf(".text\n");
    while(instr) { //stops when it is NULL.
        switch(instr -> op) {
            case I_ATRIB:
                printf("addi %s, $0, %d\n", instr -> el1.u.name, instr -> el2.u.value);
            case I_PLUS:
                printf("add %s, %s, %s\n", instr -> el1.u.name, instr -> el2.u.name, instr -> el3.u.name);
            case I_MINUS:
                printf("sub %s, %s, %s\n", instr -> el1.u.name, instr -> el2.u.name, instr -> el3.u.name);
            case I_DIV:
                printf("div %s, %s\n", instr -> el2.u.name, instr -> el3.u.name);
                printf("mfhi %s\n", instr -> el1.u.name);
            case I_MULT:
                printf("mul %s, %s\n", instr -> el2.u.name, instr -> el3.u.name);
                printf("mfhi %s\n", instr -> el1.u.name);
            case I_MOD:
                printf("div %s, %s\n", instr -> el2.u.name, instr -> el3.u.name);
                printf("mflo %s\n", instr -> el1.u.name);
            case I_LABEL:
                printf("label %s:\n", el1.u.name);
            case I_GOTO:
                printf("j %s\n", el1.u.name);
            case I_IFE:
                printf("beq %s, %s, %s\n", instr -> el1.u.name, instr -> el2.u.name, instr -> el3.u.name);
                printf("j %s\n", instr -> el4.u.name);
            case I_IFNE:
                printf("bne %s, %s, %s\n", instr -> el1.u.name, instr -> el2.u.name, instr -> el3.u.name);
                printf("j %s\n", instr -> el4.u.name);
            case I_IFG:
                printf("bgtz %s, %s, %s\n", instr -> el1.u.name, instr -> el2.u.name, instr -> el3.u.name);
                printf("j %s\n", instr -> el4.u.name);
            case I_IFGE:
            	printf("bgez %s, %s, %s\n", instr -> el1.u.name, instr -> el2.u.name, instr -> el3.u.name);
            	printf("j %s\n", instr -> el4.u.name);
            case I_IFL:
                printf("bltz %s %s %s\n", instr -> el1.u.name, instr -> el2.u.name, instr -> el3.u.name);
                printf("j %s\n", instr -> el4.u.name);
            case I_IFLE:
                printf("blez %s\n", instr -> el1.u.name, instr -> el2.u.name, instr -> el3.u.name);
                printf("j %s\n", instr -> el4.u.name);
            case I_PRINT:
            	printf("li $v0, 1\n");
            	printf("lw $a0, %s\n", instr -> el1.u.name);
            	printf("syscall\n");
            case I_READ:
            	printf("li $v0, 5\n");
            	printf("syscall\n");
            	printf("sw $v0, %s\n", instr -> el1.u.name);

        }
        instr = instr -> next;
    }
    //End of text section
}

// typedef enum { ATRIB, PLUS, MINUS, DIV, MULT, MOD, LABEL, GOTO, IFE, IFNE, IFG, IFGE, IFL, IFLE } OpKind;
//Useful document that contains the format of all instructions in the MIPS format.
