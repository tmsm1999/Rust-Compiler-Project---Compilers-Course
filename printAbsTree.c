#include <stdio.h>
#include "parser.h"
#include "printAbsTree.h"

#define PRINTTABS(t) for(int i = 0; i < t; i++) putchar('\t');

void printExpr(Expr* exp)
{
  for(int i = 0; i < exp->negative; i++)
    printf("-(");

  switch(exp->kind)
  {
    case E_VAR:
      printf("%s", exp->attr.var);
      break;

    case E_INTEGER:
      printf("%d", exp->attr.value);
      break;

    case E_OPERATION:
      putchar('('); printExpr(exp->attr.op.left); putchar(')');
      switch(exp->attr.op.operator)
      {
        case PLUS:  printf(" + "); break;
        case MINUS: printf(" - "); break;
        case MULT:  printf(" * "); break;
        case DIV:   printf(" / "); break;
        case MOD:   printf(" %% "); break;
      }
      putchar('('); printExpr(exp->attr.op.right); putchar(')');
      break;
  }

  for(int i = 0; i < exp->negative; i++)
    putchar(')');
}

void printBoolExpr(BoolExpr* boolexpr)
{
  for(int i = 0; i < boolexpr->negation; i++)
    printf("NOT(");

  switch(boolexpr->kind)
  {
    case BE_VAR:
      printf("%s", boolexpr->attr.var);
      break;

    case BE_VALUE:
      switch(boolexpr->attr.value)
      {
        case 0: printf("FALSE"); break;
        case 1: printf("TRUE"); break;
      }
      break;

    case BE_OPERATION:
      putchar('('); printExpr(boolexpr->attr.op.left); putchar(')');
      switch(boolexpr->attr.op.operator)
      {
        case EQ: printf(" == "); break;
        case NE: printf(" != "); break;
        case LT: printf(" < "); break;
        case GT: printf(" > "); break;
        case LE: printf(" <= "); break;
        case GE: printf(" >= "); break;
      }
      putchar('('); printExpr(boolexpr->attr.op.right); putchar(')');
      break;

    case BE_LOGIC:
      putchar('('); printBoolExpr(boolexpr->attr.logic.left); putchar(')');
      switch(boolexpr->attr.logic.operator)
      {
        case OR:  printf(" OR "); break;
        case AND: printf(" AND "); break;
      }
      putchar('('); printBoolExpr(boolexpr->attr.logic.right); putchar(')');
      break;
  }

  for(int i = 0; i < boolexpr->negation; i++)
    putchar(')');
}

void printCmd(Cmd* command, int tabs)
{
  PRINTTABS(tabs);

  switch(command->kind)
  {
    case C_ASSIGN_EXPR:
      printf("%s = ", command->attr.assignexpr.var);
      printExpr(command->attr.assignexpr.value); printf(";\n");
      break;

    case C_ASSIGN_BOOL:
      printf("%s = ", command->attr.assignbool.var);
      printBoolExpr(command->attr.assignbool.value); printf(";\n");
      break;

    case C_IF:
      printf("IF"); putchar('('); printBoolExpr(command->attr.ifthen.cond); putchar(')');
      putchar('\n');
      PRINTTABS(tabs); printf("{\n");
      printCmd(command->attr.ifthen.cmd, tabs+1);
      PRINTTABS(tabs); printf("}\n");
      break;

    case C_IF_ELSE:
      printf("IF"); putchar('('); printBoolExpr(command->attr.ifthenelse.cond); putchar(')');
      putchar('\n');
      PRINTTABS(tabs); printf("{\n");
      printCmd(command->attr.ifthenelse.cmd1, tabs+1);
      PRINTTABS(tabs); printf("}\n");
      PRINTTABS(tabs);
      printf("ELSE");
      putchar('\n');
      PRINTTABS(tabs); printf("{\n");
      printCmd(command->attr.ifthenelse.cmd2, tabs+1);
      PRINTTABS(tabs); printf("}\n");
      break;

    case C_WHILE:
      printf("WHILE"); putchar('('); printBoolExpr(command->attr.whileloop.cond); putchar(')');
      putchar('\n');
      PRINTTABS(tabs); printf("{\n");
      printCmd(command->attr.whileloop.cmd, tabs+1);
      PRINTTABS(tabs); printf("}\n");
      break;

    case C_PRINT:
      printf("PRINTLN(%s)", command->attr.println.var); printf(";\n");
      break;

    case C_READ:
      printf("READLINE(%s)", command->attr.readline.var); printf(";\n");
      break;
  }

  if(command->nextcmd != NULL)
    printCmd(command->nextcmd, tabs);
}