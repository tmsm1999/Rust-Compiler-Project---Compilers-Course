// AST definitions

#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr
{
  int negative; // number of MINUS's ('-') preceding the expression

  enum
  { 
    E_VAR,
    E_INTEGER,
    E_OPERATION
  } kind;
  
  union
  {
    char* var; // for integer variables

    int value; // for integer values

    struct
    { 
      int operator; // PLUS, MINUS, etc 
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
  } attr;
};

typedef struct _Expr Expr; // Convenience typedef

// AST for boolean expressions
struct _BoolExpr
{
  int negation; // number of NOT's ('!') preceding the boolean expression

  enum
  { 
    BE_VAR,
    BE_VALUE,
    BE_OPERATION,
    BE_LOGIC
  } kind;

  union
  {
    char* var; // for boolean variables

    int value; // for true(1)/false(0) values

    struct
    { 
      int operator; // EQ, NE, LT, etc
      Expr* left;
      Expr* right;
    } op; // to compare expressions

    struct
    {
      int operator; // &, |
      struct _BoolExpr* left;
      struct _BoolExpr* right;
    } logic; // for logic operations
  } attr;
};

typedef struct _BoolExpr BoolExpr; // Convenience typedef

// AST for commands
struct _Cmd
{
  struct _Cmd* nextcmd; // command/sequence of commands that come after this one (if any)

  enum
  { 
    C_ASSIGN_EXPR,
    C_ASSIGN_BOOL,
    C_IF,
    C_IF_ELSE,
    C_WHILE,
    C_PRINT,
    C_READ
  } kind;

  union
  {
    struct
    {
      char* var;
      Expr* value;
    } assignexpr; // for integer assignments

    struct
    {
      char* var;
      BoolExpr* value;
    } assignbool; // for boolean assignments

    struct
    {
      BoolExpr* cond;
      struct _Cmd* cmd;
    } ifthen; // for if then conditional commands

    struct
    {
      BoolExpr* cond;
      struct _Cmd* cmd1;
      struct _Cmd* cmd2;
    } ifthenelse; // for if then else conditional commands

    struct
    {
      BoolExpr* cond;
      struct _Cmd* cmd;
    } whileloop; // for while loops

    struct
    {
      char* var;
    } println; // for the println function

    struct
    {
      char* var;
    } readline; // for the read_line function
  } attr;
};

typedef struct _Cmd Cmd; // Convenience typedef

// Constructor functions (implementation ast.c)
Expr* ast_var(char var[]);
Expr* ast_integer(int v);
Expr* ast_operation(int operator, Expr* left, Expr* right);

BoolExpr* ast_boolVar(char var[]);
BoolExpr* ast_boolVal(int v);
BoolExpr* ast_boolExpr(int operator, Expr* left, Expr* right);
BoolExpr* ast_logicExpr(int operator, BoolExpr* left, BoolExpr* right);

Cmd* ast_assign_expr(char var[], Expr* v);
Cmd* ast_assign_boolexpr(char var[], BoolExpr* v);
Cmd* ast_ifthen(BoolExpr* cond, Cmd* cmd);
Cmd* ast_ifthenelse(BoolExpr* cond, Cmd* cmd1, Cmd* cmd2);
Cmd* ast_whileloop(BoolExpr* cond, Cmd* cmd);
Cmd* ast_println(char var[]);
Cmd* ast_readline(char var[]);

#endif