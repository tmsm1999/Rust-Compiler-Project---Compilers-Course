// AST constructor functions

#include <stdlib.h> // for malloc
#include <string.h> // for strdup
#include "ast.h" // AST header

Expr* ast_var(char var[])
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_VAR;
  node->attr.var = strdup(var);
  node->negative = 0;
  return node;
}

Expr* ast_integer(int v)
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  node->negative = 0;
  return node;
}

Expr* ast_operation(int operator, Expr* left, Expr* right)
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  node->negative = 0;
  return node;
}

BoolExpr* ast_boolVar(char var[])
{
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = E_VAR;
  node->attr.var = strdup(var);
  node->negation = 0;
  return node;
}

BoolExpr* ast_boolVal(int v)
{
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = BE_VALUE;
  node->attr.value = v;
  node->negation = 0;
  return node;
}

BoolExpr* ast_boolExpr(int operator, Expr* left, Expr* right)
{
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = BE_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  node->negation = 0;
  return node;
}

BoolExpr* ast_logicExpr(int operator, BoolExpr* left, BoolExpr* right)
{
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = BE_LOGIC;
  node->attr.logic.operator = operator;
  node->attr.logic.left = left;
  node->attr.logic.right = right;
  node->negation = 0;
  return node;
}

Cmd* ast_assign_expr(char var[], Expr* v)
{
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_ASSIGN_EXPR;
  node->attr.assignexpr.var = strdup(var);
  node->attr.assignexpr.value = v;
  node->nextcmd = NULL;
  return node;
}

Cmd* ast_assign_boolexpr(char var[], BoolExpr* v)
{
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_ASSIGN_BOOL;
  node->attr.assignbool.var = strdup(var);
  node->attr.assignbool.value = v;
  node->nextcmd = NULL;
  return node;
}

Cmd* ast_ifthen(BoolExpr* cond, Cmd* cmd)
{
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_IF;
  node->attr.ifthen.cond = cond;
  node->attr.ifthen.cmd = cmd;
  node->nextcmd = NULL;
  return node;
}

Cmd* ast_ifthenelse(BoolExpr* cond, Cmd* cmd1, Cmd* cmd2)
{
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_IF_ELSE;
  node->attr.ifthenelse.cond = cond;
  node->attr.ifthenelse.cmd1 = cmd1;
  node->attr.ifthenelse.cmd2 = cmd2;
  node->nextcmd = NULL;
  return node;
}

Cmd* ast_whileloop(BoolExpr* cond, Cmd* cmd)
{
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_WHILE;
  node->attr.whileloop.cond = cond;
  node->attr.whileloop.cmd = cmd;
  node->nextcmd = NULL;
  return node;
}

Cmd* ast_println(char var[])
{
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_PRINT;
  node->attr.println.var = strdup(var);
  node->nextcmd = NULL;
  return node;
}

Cmd* ast_readline(char var[])
{
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_READ;
  node->attr.readline.var = strdup(var);
  node->nextcmd = NULL;
  return node;
}