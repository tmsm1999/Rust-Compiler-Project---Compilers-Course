%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ 		{  			}
"//".* 		{  			}
\n 			{ yyline++; }

[0-9]+ { yylval.intValue = atoi(yytext); return INT; }

"+" { return PLUS; 	}
"-" { return MINUS; }
"*" { return MULT; 	}
"/" { return DIV; 	}
"%" { return MOD; 	}

"==" 	{ return EQ; 	}
"!=" 	{ return NE; 	}
"<" 	{ return LT; 	}
">" 	{ return GT; 	}
"<=" 	{ return LE; 	}
">=" 	{ return GE; 	}
"true" 	{ return TRUE; 	}
"false" { return FALSE; }
"!" 	{ return NOT; 	}
"||" 	{ return OR; 	}
"&&" 	{ return AND; 	}

"=" { return ASSIGN; 	}
";" { return SEMCOL; 	}
"{" { return LBRACK; 	}
"}" { return RBRACK; 	}
"(" { return LPAR; 		}
")" { return RPAR; 		}

"while" 	{ return WHILE; }
"if" 		{ return IF; 	}
"else" 		{ return ELSE; 	}
"let" 		{ return LET; 	}
"fn" 		{ return FUNC; 	}
"main" 		{ return MAIN; 	}
"println" 	{ return PRINT; }
"read_line" { return READ; 	}

[A-Za-z][0-9A-Za-z_]* { yylval.varValue = strdup(yytext); return VAR; }

. { yyerror("unexpected character"); }
%%
