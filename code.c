#include "parser.h"
#include "code.h"
#include <stdlib.h> // for malloc
#include <string.h> // for strdup
#include <stdio.h>

int var_counter_t = 0;
int var_counter_s = 0;
int label_counter = 0;

Atom* atom_value(int value)
{
	Atom* atom = (Atom*) malloc(sizeof(Atom));
	atom->kind = NUMBER;
	atom->u.value = value;
	return atom;
}

Atom* atom_name(char* name)
{
	Atom* atom = (Atom*) malloc(sizeof(Atom));
	atom->kind = STRING;
	atom->u.name = strdup(name);
	return atom;
}

Atom* atom_empty()
{
	Atom* atom = (Atom*) malloc(sizeof(Atom));
	atom->kind = EMPTY;
	return atom;
}

Instr* mkInstr(OpKind op, Atom* el1, Atom* el2, Atom* el3, Atom* el4)
{
	Instr* instr = (Instr*) malloc(sizeof(Instr));
	instr->op = op;
	instr->el1 = el1;
	instr->el2 = el2;
	instr->el3 = el3;
	instr->el4 = el4;
	instr->next = NULL;
	return instr;
}

InstrList* mkInstrList(Instr* head, InstrList* tail)
{
	InstrList* list = (InstrList*) malloc(sizeof(InstrList));

	if(tail == NULL)
	{
		list->first = head;
		list->last = head;
	}

	else
	{
		head->next = tail->first;
		list->first = head;
		list->last = tail->last;
	}

	return list;
}

InstrList* append(InstrList* l1, InstrList* l2)
{
	if(l1 == NULL) return l2;
	if(l2 == NULL) return l1;

	InstrList* list = (InstrList*) malloc(sizeof(InstrList));
	list->first = l1->first;
	l1->last->next = l2->first;
	list->last = l2->last;

	return list;
}

void printAtom(Atom* a)
{
	switch(a->kind)
	{
		case STRING:
		printf("%s", a->u.name);
		break;

		case NUMBER:
		printf("%d", a->u.value);
		break;

		case EMPTY:
		printf("EMPTY");
		break;
	}
}

void printInstr(Instr* instr)
{
	switch(instr->op)
	{
		case I_ATRIB:
		printf("ATRIB,");
		break;

		case I_PLUS:
		printf("PLUS,");
		break;

		case I_MINUS:
		printf("MINUS,");
		break;

		case I_DIV:
		printf("DIV,");
		break;

		case I_MULT:
		printf("MULT,");
		break;

		case I_MOD:
		printf("MOD,");
		break;

		case I_LABEL:
		printf("LABEL,");
		break;

		case I_GOTO:
		printf("GOTO,");
		break;

		case I_IFEQ:
		printf("IFE,");
		break;

		case I_IFNE:
		printf("IFNE,");
		break;

		case I_IFGT:
		printf("IFGT,");
		break;

		case I_IFGE:
		printf("IFGE,");
		break;

		case I_IFLT:
		printf("IFLT,");
		break;

		case I_IFLE:
		printf("IFLE,");
		break;

		case I_PRINT:
		printf("PRINT,");
		break;

		case I_READ:
		printf("READ,");
		break;

		case I_LOAD:
		printf("LOAD,");
		break;

		case I_STORE:
		printf("STORE,");
		break;
	}

	printAtom(instr->el1);
	printf(",");
	printAtom(instr->el2);
	printf(",");
	printAtom(instr->el3);
	printf(",");
	printAtom(instr->el4);
}

typedef struct
{
	Instr* first;
	Instr* last;
} _InstrList;

typedef struct _InstrList InstrList;

void printInstrList(InstrList* list)
{
	Instr* instr = list->first;
	printInstr(instr);
	putchar('\n');

	while((instr = instr->next))
	{
		printInstr(instr);
		putchar('\n');
	}
}

//========================================================================================================================

char* newVar()
{
	char buffer[20];

	if(var_counter_t <= 9)
	{
		sprintf(buffer, "$t%d", var_counter_t++);
	}
	else if(var_counter_s <= 7)
	{
		sprintf(buffer, "$s%d", var_counter_s++);
	}
	else
	{
		sprintf(buffer, "$t%d", var_counter_t++);
	}

	return strdup(buffer);
}

char* newLabel()
{
	char buffer[20];
	sprintf(buffer, "l%d", label_counter);
	label_counter++;
	return strdup(buffer);
}

OpKind map_operator(int op)
{
  switch(op)
  {
  	case PLUS: return I_PLUS;
  	case MINUS: return I_MINUS;
  	case DIV: return I_DIV;
  	case MULT: return I_MULT;
  	case MOD: return I_MOD;
  	case EQ: return I_IFEQ;
  	case NE: return I_IFNE;
  	case GT: return I_IFGT;
  	case GE: return I_IFGE;
  	case LT: return I_IFLT;
  	case LE: return I_IFLE;
  	default: return -1;
  }
}

InstrList* compileExpr(Expr* exp, char* reg)
{
	switch(exp->kind)
	{
		case E_VAR:
		{
			InstrList* code = mkInstrList(mkInstr(I_LOAD, atom_name(reg), atom_name(exp->attr.var), atom_empty(), atom_empty()), NULL);
			return code;
		}

		case E_INTEGER:
		{
			InstrList* code = mkInstrList(mkInstr(I_ATRIB, atom_name(reg), atom_value(exp->attr.value), atom_empty(), atom_empty()), NULL);
			return code;
		}

		case E_OPERATION:
		{
			char* r1 = newVar();
			InstrList* l1 = compileExpr(exp->attr.op.left, r1);
			char* r2 = newVar();
			InstrList* l2 = compileExpr(exp->attr.op.right, r2);
			InstrList* l3 = append(l1, l2);
			InstrList* l4 = append(l3, mkInstrList(mkInstr(map_operator(exp->attr.op.operator), atom_name(reg), atom_name(r1), atom_name(r2), atom_empty()), NULL));
			return l4;
		}
	}
}

InstrList* compileBool(char* l_true, char* l_false, BoolExpr* boolexp)
{
	switch(boolexp->kind)
	{
		case BE_VAR:
		{
			char* t1 = newVar();
			InstrList* l1 = mkInstrList(mkInstr(I_LOAD, atom_name(t1), atom_name(boolexp->attr.var), atom_empty(), atom_empty()), NULL);
			InstrList* code = append(l1, mkInstrList(mkInstr(I_IFNE, atom_name(t1), atom_name("$zero"), atom_name(l_true), atom_name(l_false)), NULL));
			return code;
		}

		case BE_VALUE:
		{
			char* t1 = newVar();
			InstrList* l1 = mkInstrList(mkInstr(I_LOAD, atom_name(t1), atom_value(boolexp->attr.value), atom_empty(), atom_empty()), NULL);
			InstrList* code = append(l1, mkInstrList(mkInstr(I_IFNE, atom_name(t1), atom_name("$zero"), atom_name(l_true), atom_name(l_false)), NULL));
			return code;
		}

		case BE_OPERATION:
		{
			char* r1 = newVar();
			InstrList* l1 = compileExpr(boolexp->attr.op.left, r1);
			char* r2 = newVar();
			InstrList* l2 = compileExpr(boolexp->attr.op.right, r2);
			InstrList* l3 = append(l1, l2);
			InstrList* l4 = append(l3, mkInstrList(mkInstr(map_operator(boolexp->attr.op.operator), atom_name(r1), atom_name(r2), atom_name(l_true), atom_name(l_false)), NULL));
			return l4;
		}

		case BE_LOGIC:
		{
			return NULL;
		}
	}
}

InstrList* compileCmd(Cmd* cmd)
{
	InstrList* code;

	switch(cmd->kind)
	{
		case C_ASSIGN_EXPR:
		{
			char* r1 = newVar();
			InstrList* l1 = compileExpr(cmd->attr.assignexpr.value, r1);
			code = append(l1, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignexpr.var), atom_empty(), atom_empty()), NULL));
			break;
		}

		case C_ASSIGN_BOOL:
		{
			//InstrList* code = compileBool(cmd->attr.assignexpr.value, cmd->attr.assignbool.var);
			return NULL;
		}

		case C_IF:
		{
			char* l_true = newLabel();
			char* l_false = newLabel();
			InstrList* cond = compileBool(l_true, l_false, cmd->attr.ifthen.cond);
			InstrList* body = compileCmd(cmd->attr.ifthen.cmd);
			code = append(append(cond, mkInstrList(mkInstr(I_LABEL, atom_name(l_true), atom_empty(), atom_empty(), atom_empty()), NULL)), append(body, mkInstrList(mkInstr(I_LABEL, atom_name(l_false), atom_empty(), atom_empty(), atom_empty()), NULL)));
			break;
		}

		case C_IF_ELSE:
		{
			char* l_true = newLabel();
			char* l_false = newLabel();
			InstrList* cond = compileBool(l_true, l_false, cmd->attr.ifthenelse.cond);
			InstrList* body1 = compileCmd(cmd->attr.ifthenelse.cmd1);
			InstrList* body2 = compileCmd(cmd->attr.ifthenelse.cmd2);
			code = append(cond, append(append(mkInstrList(mkInstr(I_LABEL, atom_name(l_true), atom_empty(), atom_empty(), atom_empty()), NULL), body1), append(mkInstrList(mkInstr(I_LABEL, atom_name(l_false), atom_empty(), atom_empty(), atom_empty()), NULL), body2)));
			break;
		}

		case C_WHILE:
		{
			char* l_top = newLabel();
			char* l_true = newLabel();
			char* l_false = newLabel();
			InstrList* cond = compileBool(l_true, l_false, cmd->attr.whileloop.cond);
			InstrList* body = compileCmd(cmd->attr.whileloop.cmd);
			InstrList* l1 = mkInstrList(mkInstr(I_LABEL, atom_name(l_top), atom_empty(), atom_empty(), atom_empty()), cond);
			InstrList* l2 = mkInstrList(mkInstr(I_LABEL, atom_name(l_true), atom_empty(), atom_empty(), atom_empty()), body);
			InstrList* l3 = mkInstrList(mkInstr(I_GOTO, atom_name(l_top), atom_empty(), atom_empty(), atom_empty()), mkInstrList(mkInstr(I_LABEL, atom_name(l_false), atom_empty(), atom_empty(), atom_empty()), NULL));
			code = append(append(l1, l2), l3);
			break;
		}

		case C_PRINT:
		{
			code = mkInstrList(mkInstr(I_PRINT, atom_name(cmd->attr.println.var), atom_empty(), atom_empty(), atom_empty()), NULL);
			break;
		}

		case C_READ:
		{
			code = mkInstrList(mkInstr(I_READ, atom_name(cmd->attr.readline.var), atom_empty(), atom_empty(), atom_empty()), NULL);
			break;
		}
	}

	if(cmd->nextcmd == NULL)
		return code;
	else
		return append(code, compileCmd(cmd->nextcmd));
}