// Tokens
%token
  INT     // number
  PLUS    // +
  MINUS   // -
  MULT    // *
  DIV     // /
  MOD     // %
  EQ      // ==
  NE      // !=
  LT      // <
  GT      // >
  LE      // <=
  GE      // >=
  TRUE    // true
  FALSE   // false
  WHILE   // while
  IF      // if
  ELSE    // else
  ASSIGN  // =
  SEMCOL  // ;
  LBRACK  // {
  RBRACK  // }
  LET     // let declaration
  VAR     // variable names
  FUNC    // functions
  MAIN    // main
  LPAR    // (
  RPAR    // )
  NOT     // !
  OR      // ||
  AND     // &&
  PRINT   // println
  READ    // read_line

// Operators associativity & precedence
%left OR
%left AND
%left EQ NE LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%precedence UMINUS NOT

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union
{
  int intValue;
  Expr* exprValue;
  BoolExpr* boolExprValue;
  Cmd* cmdValue;
  char* varValue;
}

%type <intValue> INT
%type <exprValue> expr
%type <boolExprValue> boolexpr
%type <cmdValue> seqcmd
%type <cmdValue> cmd
%type <varValue> VAR

// Declarations that go into parser.c and parser.h
%code requires
{
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"
  #include "printAbsTree.h"
  #include "hash.h"
  #include "list.h"

  extern int yylex();
  extern int yyline;
  extern char* yytext;
  extern FILE* yyin;
  extern void yyerror(const char* msg);
  extern void error_message(const char* msg);
  Cmd* root;
  HASH_TABLE variables;
}

%%
program:
  FUNC MAIN LPAR RPAR LBRACK seqcmd RBRACK { root = $6;   }
| FUNC MAIN LPAR RPAR LBRACK RBRACK        { root = NULL; }
;

seqcmd:
  cmd seqcmd  { $$ = $1; $$->nextcmd = $2;  }
| cmd         { $$ = $1;                    }
;

cmd:
  LET VAR ASSIGN expr SEMCOL                                  { $$ = ast_assign_expr($2, $4); insert(variables, $2);                                      }
| LET VAR ASSIGN boolexpr SEMCOL                              { $$ = ast_assign_boolexpr($2, $4); insert(variables, $2);                                  }
| READ LPAR VAR RPAR SEMCOL                                   { $$ = ast_readline($3); if(lookup(variables, $3) == NULL) { error_message($3); return 1; } }
| PRINT LPAR VAR RPAR SEMCOL                                  { $$ = ast_println($3); if(lookup(variables, $3) == NULL) { error_message($3); return 1; }  }
| IF boolexpr LBRACK seqcmd RBRACK ELSE LBRACK seqcmd RBRACK  { $$ = ast_ifthenelse($2, $4, $8);                                                          }
| IF boolexpr LBRACK seqcmd RBRACK                            { $$ = ast_ifthen($2, $4);                                                                  }
| WHILE boolexpr LBRACK seqcmd RBRACK                         { $$ = ast_whileloop($2, $4);                                                               }
;

expr:
  LPAR expr RPAR          { $$ = $2;                                                                              }
| VAR                     { $$ = ast_var($1); if(lookup(variables, $1) == NULL) { error_message($1); return 1; }  }  
| INT                     { $$ = ast_integer($1);                                                                 }
| expr PLUS expr          { $$ = ast_operation(PLUS, $1, $3);                                                     }
| expr MINUS expr         { $$ = ast_operation(MINUS, $1, $3);                                                    }
| expr MULT expr          { $$ = ast_operation(MULT, $1, $3);                                                     }
| expr DIV expr           { $$ = ast_operation(DIV, $1, $3);                                                      }
| expr MOD expr           { $$ = ast_operation(MOD, $1, $3);                                                      }
| MINUS expr %prec UMINUS { $$ = $2; $$->negative++;                                                              }
;

boolexpr:
  LPAR boolexpr RPAR    { $$ = $2; }
| VAR                   { $$ = ast_boolVar($1); if(lookup(variables, $1) == NULL) { error_message($1); return 1; }  }  
| TRUE                  { $$ = ast_boolVal(1);                                                                      }
| FALSE                 { $$ = ast_boolVal(0);                                                                      }
| expr EQ expr          { $$ = ast_boolExpr(EQ, $1, $3);                                                            }
| expr NE expr          { $$ = ast_boolExpr(NE, $1, $3);                                                            }
| expr LT expr          { $$ = ast_boolExpr(LT, $1, $3);                                                            }
| expr GT expr          { $$ = ast_boolExpr(GT, $1, $3);                                                            }
| expr LE expr          { $$ = ast_boolExpr(LE, $1, $3);                                                            }
| expr GE expr          { $$ = ast_boolExpr(GE, $1, $3);                                                            }
| NOT boolexpr          { $$ = $2; $$->negation++;                                                                  }
| boolexpr OR boolexpr  { $$ = ast_logicExpr(OR, $1, $3);                                                           }
| boolexpr AND boolexpr { $$ = ast_logicExpr(AND, $1, $3);                                                          }
;
%%

void error_message(const char* err)
{
  printf("Line %d: Variable %s not initialised\n", yyline, err);
}

void yyerror(const char* err)
{
  printf("Line %d: %s - '%s'\n", yyline, err, yytext);
}