#include <stdio.h>
#include "parser.h"
#include "printAbsTree.h"

int main(int argc, char** argv)
{
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
      printCmd(root, 0);
  }

  return 0;
}
