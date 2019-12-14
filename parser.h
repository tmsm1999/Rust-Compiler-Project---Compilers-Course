/* A Bison parser, made by GNU Bison 3.4.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 66 "parser.bison"

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

#line 66 "parser.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    PLUS = 259,
    MINUS = 260,
    MULT = 261,
    DIV = 262,
    MOD = 263,
    EQ = 264,
    NE = 265,
    LT = 266,
    GT = 267,
    LE = 268,
    GE = 269,
    TRUE = 270,
    FALSE = 271,
    WHILE = 272,
    IF = 273,
    ELSE = 274,
    ASSIGN = 275,
    SEMCOL = 276,
    LBRACK = 277,
    RBRACK = 278,
    LET = 279,
    VAR = 280,
    FUNC = 281,
    MAIN = 282,
    LPAR = 283,
    RPAR = 284,
    NOT = 285,
    OR = 286,
    AND = 287,
    PRINT = 288,
    READ = 289,
    UMINUS = 290
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 49 "parser.bison"

  int intValue;
  Expr* exprValue;
  BoolExpr* boolExprValue;
  Cmd* cmdValue;
  char* varValue;

#line 121 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
