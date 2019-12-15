#include <stdio.h>
#include "parser.h"
#include "printAbsTree.h"
#include "code.h"
#include "mips_code.h"

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
