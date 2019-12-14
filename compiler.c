#include <stdio.h>
#include "parser.h"
#include "printAbsTree.h"
#include "code.h"

void printMIPS(InstrList* intermCode) {
    Instr* instr = intermCode -> first;

    printf(".data\n"); //begining of data section
    for(int i = 0; i < HASH_SIZE; i++) {
      if(variables -> array[i] != NULL) {
        LIST_HT cur = variables -> array[i];
        while(cur != NULL) {
          printf("%s: .space 4\n", cur -> variable);
          cur = cur->next;
        }
      } 
    }
    //end of data section


    //begining of text section
    printf(".text\n");
    while(instr) { //stops when it is NULL.
        switch(instr -> op) {
            case I_ATRIB:
                printf("addi %s, $0, %d\n", instr -> el1->u.name, instr -> el2 -> u.value);
                break;
            case I_PLUS:
                printf("add %s, %s, %s\n", instr -> el1->u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);
                break;
            case I_MINUS:
                printf("sub %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);
                break;
            case I_DIV:
                printf("div %s, %s\n", instr -> el2 -> u.name, instr -> el3 -> u.name);
                printf("mflo %s\n", instr -> el1 -> u.name);
                break;
            case I_MULT:
                printf("mul %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);
                break;
            case I_MOD:
                printf("div %s, %s\n", instr -> el2 -> u.name, instr -> el3 -> u.name);
                printf("mfhi %s\n", instr -> el1 -> u.name);
                break;
            case I_LABEL:
                printf("%s:\n", instr -> el1 -> u.name);
                break;
            case I_GOTO:
                printf("j %s\n", instr -> el1 -> u.name);
                break;
            case I_BEQ:
                printf("seq %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);               
                break;
            case I_BNE:
                printf("sne %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);               
                break;
            case I_BGT:
                printf("sgt %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);               
                break;
            case I_BGE:
                printf("sge %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);               
                break;
            case I_BLT:
               	printf("slt %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);               
                break;
            case I_BLE:
                printf("sle %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);               
                break;
            case I_PRINT:
                printf("li $v0, 1\n");
                printf("lw $a0, %s\n", instr -> el1->u.name);
                printf("syscall\n");
                break;
            case I_READ:
                printf("li $v0, 5\n");
                printf("syscall\n");
                printf("sw $v0, %s\n", instr -> el1 -> u.name);
                break;
            case I_LOAD:
                printf("lw %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name);
                break;
            case I_STORE:
                printf("sw %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name);
                break;
            case I_IF:
            	printf("bne %s, $zero, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name);
            	printf("j %s\n", instr -> el3 -> u.name);
            	break;
            case I_AND:
            	printf("and %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);
            	break;
            case I_OR:
            	printf("or %s, %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name, instr -> el3 -> u.name);
            	break;
            case I_NOT:
            	printf("not %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name);
            	break;
            case I_NEG:
            	printf("neg %s, %s\n", instr -> el1 -> u.name, instr -> el2 -> u.name);
            	break;
        }
        instr = instr -> next;
    }
    //End of text section
}

int main(int argc, char** argv)
{
  variables = init_table();
  --argc; ++argv;

  if (argc != 0)
  {
    yyin = fopen(*argv, "r");
    if (!yyin)
    {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin

  if (yyparse() == 0)
  {
    if(root != NULL)
    {
      //printCmd(root, 0);

      InstrList* code = compileCmd(root);

      //printInstrList(code);

      printMIPS(code);
    }
  }

  return 0;
}
