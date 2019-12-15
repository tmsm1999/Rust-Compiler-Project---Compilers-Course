#include "parser.h"
#include "code.h"
#include "mips_code.h"
#include <stdio.h> // for printf

// Translation of intermediate code to MIPS (prints the mips program)
void printMIPS(InstrList* intermCode)
{
    Instr* instr = intermCode->first;

    // Begining of data section
    printf(".data\n");

    for(int i = 0; i < HASH_SIZE; i++)
    {
        if(variables->array[i] != NULL)
        {
            LIST_HT cur = variables->array[i];
            while(cur != NULL)
            {
                printf("\t%s: .word 0\n", cur->variable);
                cur = cur->next;
            }
        }
    }
    printf("\tnewline: .asciiz \"\\n\"\n");
    // End of data section

    // Begining of text section
    printf(".text\n");
    printf("main:\n");
    while(instr)
    {
        switch(instr->op)
        {
            case I_ATRIB:
                printf("\tli %s, %d\n", instr->el1->u.name, instr->el2->u.value);
                break;
            case I_PLUS:
                printf("\tadd %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);
                break;
            case I_ADDI:
                printf("\taddi %s, %s, %d\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.value);
                break;
            case I_MINUS:           
                printf("\tsub %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);
                break;
            case I_SUBI:           
                printf("\tsubi %s, %s, %d\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.value);
                break;
            case I_DIV:         
                printf("\tdiv %s, %s\n", instr->el2->u.name, instr->el3->u.name);
                printf("\tmflo %s\n", instr->el1->u.name);
                break;
            case I_MULT:
                printf("\tmul %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);
                break;
            case I_MOD:            	
                printf("\tdiv %s, %s\n", instr->el2->u.name, instr->el3->u.name);
                printf("\tmfhi %s\n", instr->el1->u.name);
                break;
            case I_NEG:
                printf("\tneg %s, %s\n", instr->el1->u.name, instr->el2->u.name);
                break;
            case I_LABEL:
                printf("%s:\n", instr->el1->u.name);
                break;
            case I_GOTO:            	
                printf("\tj %s\n", instr->el1->u.name);
                break;
            case I_IF:
                printf("\tbne %s, $zero, %s\n", instr->el1->u.name, instr->el2->u.name);
                printf("\tj %s\n", instr->el3->u.name);
                break;
            case I_BEQ:            
                printf("\tseq %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);               
                break;
            case I_BNE:            	
                printf("\tsne %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);               
                break;
            case I_BGT:
                printf("\tsgt %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);               
                break;
            case I_BGE:            	
                printf("\tsge %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);               
                break;
            case I_BLT:            
                printf("\tslt %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);               
                break;
            case I_BLE:
                printf("\tsle %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);               
                break;
            case I_OR:
                printf("\tor %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);
                break;
            case I_ORI:
                printf("\tori %s, %s, %d\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.value);
                break;
            case I_AND:
                printf("\tand %s, %s, %s\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.name);
                break;
            case I_ANDI:
                printf("\tandi %s, %s, %d\n", instr->el1->u.name, instr->el2->u.name, instr->el3->u.value);
                break;
            case I_NOT:
                printf("\tseq %s, %s, $zero\n", instr->el1->u.name, instr->el2->u.name);
                break;
            case I_PRINT:
                printf("\tli $v0, 1\n");                
                printf("\tlw $a0, %s\n", instr->el1->u.name);              
                printf("\tsyscall\n"); 
                printf("\tli $v0, 4\n"); 
                printf("\tla $a0, newline\n");              
                printf("\tsyscall\n");
                break;
            case I_READ:
                printf("\tli $v0, 5\n");
                printf("\tsyscall\n");
                printf("\tsw $v0, %s\n", instr->el1->u.name);
                break;
            case I_LOAD:
                printf("\tlw %s, %s\n", instr->el1->u.name, instr->el2->u.name);
                break;
            case I_STORE:           
                printf("\tsw %s, %s\n", instr->el1->u.name, instr->el2->u.name);
                break;
        }

        instr = instr->next;
    }
    // End of text section
}