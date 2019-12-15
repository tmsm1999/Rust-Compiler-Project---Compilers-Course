#include "parser.h"
#include "code.h"
#include <stdlib.h> // for malloc
#include <string.h> // for strdup
#include <stdio.h> 	// for printf

//========================================================================================================================
// List of Instructions constructors and utilities (append and print)
//========================================================================================================================
// The intermediate code is represented by a list of instructions (InstrList)


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

		case I_ADDI:
			printf("ADDI,");
			break;

		case I_MINUS:
			printf("MINUS,");
			break;

		case I_SUBI:
			printf("SUBI,");
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

		case I_NEG:
			printf("NEG,");
			break;

		case I_LABEL:
			printf("LABEL,");
			break;

		case I_GOTO:
			printf("GOTO,");
			break;

		case I_IF:
			printf("IF,");
			break;

		case I_BEQ:
			printf("BEQ,");
			break;

		case I_BNE:
			printf("BNE,");
			break;

		case I_BGT:
			printf("BGT,");
			break;

		case I_BGE:
			printf("BGE,");
			break;

		case I_BLT:
			printf("BLT,");
			break;

		case I_BLE:
			printf("BLE,");
			break;

		case I_OR:
			printf("OR,");
			break;

		case I_ORI:
			printf("ORI,");
			break;

		case I_AND:
			printf("AND,");
			break;

		case I_ANDI:
			printf("ANDI,");
			break;

		case I_NOT:
			printf("NOT,");
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
// Compilation functions (from ast to intermediate code)
//========================================================================================================================

int label_counter = 0; 		// number of current labels
int var_counter_t = 0; 		// number of $t registers being used (from $t0 to $t(var_counter_t-1))
int var_counter_s = 0; 		// number of $s registers being used (from $s0 to $s(var_counter_s-1))
int var_counter_inf = 10; 	// number of registers being used beyond the existing ones (from _t10 to _t(infinite))

// Returns a new label
char* newLabel()
{
	char buffer[20];
	sprintf(buffer, "l%d", label_counter);
	label_counter++;
	return strdup(buffer);
}

// Returns a new register
// Starts by using $t registers, then $s and then starts using not existing ones (_t) that will be declared as variables in .data
char* newVar()
{
	char buffer[20];

	if(var_counter_t <= 5) // $t7, $t8 and $t9 are used as auxiliar registers to use the values of _t "registers". $t6 is used in assigning optimization
	{
		sprintf(buffer, "$t%d", var_counter_t++);
	}
	else if(var_counter_s <= 7)
	{
		sprintf(buffer, "$s%d", var_counter_s++);
	}
	else
	{
		sprintf(buffer, "_t%d", var_counter_inf++);
		insert(variables, strdup(buffer)); // insert in .data
	}

	return strdup(buffer);
}

// Maps ast operators with intermediate code OpKind
OpKind map_operator(int op)
{
	switch(op)
	{
		case PLUS: 	return I_PLUS;
		case MINUS: return I_MINUS;
		case DIV: 	return I_DIV;
		case MULT: 	return I_MULT;
		case MOD: 	return I_MOD;
		case EQ: 	return I_BEQ;
		case NE: 	return I_BNE;
		case GT: 	return I_BGT;
		case GE: 	return I_BGE;
		case LT: 	return I_BLT;
		case LE: 	return I_BLE;
		case OR: 	return I_OR;
		case AND: 	return I_AND;
		default: 	return -1;
	}
}

// Compiles expression and stores the result in register 'place'
InstrList* compileExpr(Expr* exp, char* place)
{
	InstrList* code;
	char* reg;

	if(place[0] == '_') reg = "$t7"; else reg = place; // Ran out of valid registers

	switch(exp->kind)
	{
		case E_VAR:
		{
			code = mkInstrList(mkInstr(I_LOAD, atom_name(reg), atom_name(exp->attr.var), atom_empty(), atom_empty()), NULL);
			break;
		}

		case E_INTEGER:
		{
			code = mkInstrList(mkInstr(I_ATRIB, atom_name(reg), atom_value(exp->attr.value), atom_empty(), atom_empty()), NULL);
			break;
		}

		case E_OPERATION:
		{
			// Optimization: uses addi and subi if the right expression is a constant
			if(exp->attr.op.right->kind == E_INTEGER && (exp->attr.op.operator == PLUS || exp->attr.op.operator == MINUS))
			{
				char* r1 = newVar();
				code = compileExpr(exp->attr.op.left, r1);

				if(r1[0] == '_')
				{
					code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name("$t8"), atom_name(r1), atom_empty(), atom_empty()), NULL));
					r1 = "$t8";
				}

				int value = (exp->attr.op.right->negative % 2 == 0 ? exp->attr.op.right->attr.value : -exp->attr.op.right->attr.value);
				OpKind op = (exp->attr.op.operator == PLUS ? I_ADDI : I_SUBI);
				code = append(code, mkInstrList(mkInstr(op, atom_name(reg), atom_name(r1), atom_value(value), atom_empty()), NULL));
			}

			else
			{
				char* r1 = newVar();
				code = compileExpr(exp->attr.op.left, r1);

				char* r2 = newVar();
				code = append(code, compileExpr(exp->attr.op.right, r2));

				if(r1[0] == '_')
				{
					code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name("$t8"), atom_name(r1), atom_empty(), atom_empty()), NULL));
					r1 = "$t8";
				}

				if(r2[0] == '_')
				{
					code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name("$t9"), atom_name(r2), atom_empty(), atom_empty()), NULL));
					r2 = "$t9";
				}

				code = append(code, mkInstrList(mkInstr(map_operator(exp->attr.op.operator), atom_name(reg), atom_name(r1), atom_name(r2), atom_empty()), NULL));
			}

			break;
		}
	}

	if(exp->negative % 2 != 0)
		code = append(code, mkInstrList(mkInstr(I_NEG, atom_name(reg), atom_name(reg), atom_empty(), atom_empty()), NULL));

	if(place[0] == '_')
		code = append(code, mkInstrList(mkInstr(I_STORE, atom_name(reg), atom_name(place), atom_empty(), atom_empty()), NULL));

	return code;
}

// Compiles boolean expression and stores the result in register 'place'
InstrList* compileBoolExpr(BoolExpr* boolexp, char* place)
{
	InstrList* code;
	char* reg;

	if(place[0] == '_') reg = "$t7"; else reg = place; // Ran out of valid registers

	switch(boolexp->kind)
	{
		case BE_VAR:
		{
			code = mkInstrList(mkInstr(I_LOAD, atom_name(reg), atom_name(boolexp->attr.var), atom_empty(), atom_empty()), NULL);
			break;
		}

		case BE_VALUE:
		{
			code = mkInstrList(mkInstr(I_ATRIB, atom_name(reg), atom_value(boolexp->attr.value), atom_empty(), atom_empty()), NULL);
			break;
		}

		case BE_OPERATION:
		{
			char* r1 = newVar();
			code = compileExpr(boolexp->attr.op.left, r1);

			char* r2 = newVar();
			code = append(code, compileExpr(boolexp->attr.op.right, r2));

			if(r1[0] == '_')
			{
				code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name("$t8"), atom_name(r1), atom_empty(), atom_empty()), NULL));
				r1 = "$t8";
			}

			if(r2[0] == '_')
			{
				code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name("$t9"), atom_name(r2), atom_empty(), atom_empty()), NULL));
				r2 = "$t9";
			}

			code = append(code, mkInstrList(mkInstr(map_operator(boolexp->attr.op.operator), atom_name(reg), atom_name(r1), atom_name(r2), atom_empty()), NULL));
			break;
		}

		case BE_LOGIC:
		{
			// Optimization: uses ori and andi if the right expression is a "constant" (true/false)
			if(boolexp->attr.logic.right->kind == BE_VALUE)
			{
				char* r1 = newVar();
				code = compileBoolExpr(boolexp->attr.logic.left, r1);

				if(r1[0] == '_')
				{
					code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name("$t8"), atom_name(r1), atom_empty(), atom_empty()), NULL));
					r1 = "$t8";
				}

				int value = (boolexp->attr.logic.right->negation % 2 == 0 ? boolexp->attr.logic.right->attr.value : !boolexp->attr.logic.right->attr.value);
				OpKind op = (boolexp->attr.logic.operator == OR ? I_ORI : I_ANDI);
				code = append(code, mkInstrList(mkInstr(op, atom_name(reg), atom_name(r1), atom_value(value), atom_empty()), NULL));
			}

			else
			{
				char* r1 = newVar();
				code = compileBoolExpr(boolexp->attr.logic.left, r1);

				char* r2 = newVar();
				code = append(code, compileBoolExpr(boolexp->attr.logic.right, r2));

				if(r1[0] == '_')
				{
					code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name("$t8"), atom_name(r1), atom_empty(), atom_empty()), NULL));
					r1 = "$t8";
				}

				if(r2[0] == '_')
				{
					code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name("$t9"), atom_name(r2), atom_empty(), atom_empty()), NULL));
					r2 = "$t9";
				}

				code = append(code, mkInstrList(mkInstr(map_operator(boolexp->attr.logic.operator), atom_name(reg), atom_name(r1), atom_name(r2), atom_empty()), NULL));
			}
			
			break;
		}
	}

	if(boolexp->negation % 2 != 0)
		code = append(code, mkInstrList(mkInstr(I_NOT, atom_name(reg), atom_name(reg), atom_empty(), atom_empty()), NULL));

	if(place[0] == '_')
		code = append(code, mkInstrList(mkInstr(I_STORE, atom_name(reg), atom_name(place), atom_empty(), atom_empty()), NULL));

	return code;
}

// Compiles boolean condition of if's and while's
InstrList* compileBoolCond(char* l_true, char* l_false, BoolExpr* boolexp)
{
	char* r1 = newVar();
	InstrList* l1 = compileBoolExpr(boolexp, r1);
	InstrList* code = append(l1, mkInstrList(mkInstr(I_IF, atom_name(r1), atom_name(l_true), atom_name(l_false), atom_empty()), NULL));
	return code;
}

// Compiles commands
InstrList* compileCmd(Cmd* cmd)
{
	InstrList* code;

	switch(cmd->kind)
	{
		case C_ASSIGN_EXPR:
		{
			Expr* exptop = cmd->attr.assignexpr.value;
			Expr* expleft = exptop->attr.op.left;
			Expr* expright = exptop->attr.op.right;
			bool expleftisvar = (exptop->kind == E_OPERATION ? (expleft->kind == E_VAR && strcmp(expleft->attr.var, cmd->attr.assignexpr.var) == 0 && expleft->negative % 2 == 0) : 0);
			bool exprightisvar = (exptop->kind == E_OPERATION ? (expright->kind == E_VAR && strcmp(expright->attr.var, cmd->attr.assignexpr.var) == 0 && expright->negative % 2 == 0) : 0);

			// Optimization: if the command is "x = x op expright" or "x = exprleft op x" or "x = x op x", uses the destination register in the operation itself (example: $t0 = $t0 op expright)
			if(expleftisvar || exprightisvar)
			{
				char* r1 = newVar();
				if(r1[0] == '_') r1 = "$t7";

				if(expleftisvar)
				{
					if(exprightisvar)
					{
						code = mkInstrList(mkInstr(map_operator(exptop->attr.op.operator), atom_name(r1), atom_name(r1), atom_name(r1), atom_empty()), NULL);
						code = append(code, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignexpr.var), atom_empty(), atom_empty()), NULL));
					}
					else
					{
						code = compileExpr(expright, "$t6");
						code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name(r1), atom_name(cmd->attr.assignexpr.var), atom_empty(), atom_empty()), NULL));
						code = append(code, mkInstrList(mkInstr(map_operator(exptop->attr.op.operator), atom_name(r1), atom_name(r1), atom_name("$t6"), atom_empty()), NULL));
						code = append(code, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignexpr.var), atom_empty(), atom_empty()), NULL));
					}
				}

				else if(exprightisvar)
				{
					code = compileExpr(expleft, "$t6");
					code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name(r1), atom_name(cmd->attr.assignexpr.var), atom_empty(), atom_empty()), NULL));
					code = append(code, mkInstrList(mkInstr(map_operator(exptop->attr.op.operator), atom_name(r1), atom_name("$t6"), atom_name(r1), atom_empty()), NULL));
					code = append(code, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignexpr.var), atom_empty(), atom_empty()), NULL));
				}
			}

			else
			{
				char* r1 = newVar();
				if(r1[0] == '_') r1 = "$t7";
				InstrList* l1 = compileExpr(cmd->attr.assignexpr.value, r1);

				code = append(l1, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignexpr.var), atom_empty(), atom_empty()), NULL));
			}

			break;
		}

		case C_ASSIGN_BOOL:
		{
			BoolExpr* boolexptop = cmd->attr.assignbool.value;
			BoolExpr* boolexpleft = boolexptop->attr.logic.left;
			BoolExpr* boolexpright = boolexptop->attr.logic.right;
			bool boolexpleftisvar = (boolexptop->kind == BE_LOGIC ? (boolexpleft->kind == BE_VAR && strcmp(boolexpleft->attr.var, cmd->attr.assignbool.var) == 0 && boolexpleft->negation % 2 == 0) : 0);
			bool boolexprightisvar = (boolexptop->kind == BE_LOGIC ? (boolexpright->kind == BE_VAR && strcmp(boolexpright->attr.var, cmd->attr.assignbool.var) == 0 && boolexpright->negation % 2 == 0) : 0);

			// Optimization: if the command is "x = x op boolexpright" or "x = boolexprleft op x" or "x = x op x", uses the destination register in the operation itself (example: $t0 = $t0 op boolexpright)
			if(boolexpleftisvar || boolexprightisvar)
			{
				char* r1 = newVar();
				if(r1[0] == '_') r1 = "$t7";

				if(boolexpleftisvar)
				{
					if(boolexprightisvar)
					{
						code = mkInstrList(mkInstr(map_operator(boolexptop->attr.logic.operator), atom_name(r1), atom_name(r1), atom_name(r1), atom_empty()), NULL);
						code = append(code, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignbool.var), atom_empty(), atom_empty()), NULL));
					}
					else
					{
						code = compileBoolExpr(boolexpright, "$t6");
						code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name(r1), atom_name(cmd->attr.assignbool.var), atom_empty(), atom_empty()), NULL));
						code = append(code, mkInstrList(mkInstr(map_operator(boolexptop->attr.logic.operator), atom_name(r1), atom_name(r1), atom_name("$t6"), atom_empty()), NULL));
						code = append(code, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignbool.var), atom_empty(), atom_empty()), NULL));
					}
				}

				else if(boolexprightisvar)
				{
					code = compileBoolExpr(boolexpleft, "$t6");
					code = append(code, mkInstrList(mkInstr(I_LOAD, atom_name(r1), atom_name(cmd->attr.assignbool.var), atom_empty(), atom_empty()), NULL));
					code = append(code, mkInstrList(mkInstr(map_operator(boolexptop->attr.logic.operator), atom_name(r1), atom_name("$t6"), atom_name(r1), atom_empty()), NULL));
					code = append(code, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignbool.var), atom_empty(), atom_empty()), NULL));
				}
			}

			else
			{
				char* r1 = newVar();
				if(r1[0] == '_') r1 = "$t7";
				InstrList* l1 = compileBoolExpr(cmd->attr.assignbool.value, r1);

				code = append(l1, mkInstrList(mkInstr(I_STORE, atom_name(r1), atom_name(cmd->attr.assignbool.var), atom_empty(), atom_empty()), NULL));
			}

			break;
		}

		case C_IF:
		{
			char* l_true = newLabel();
			char* l_false = newLabel();
			InstrList* cond = compileBoolCond(l_true, l_false, cmd->attr.ifthen.cond);
			InstrList* body = compileCmd(cmd->attr.ifthen.cmd);
			code = append(append(cond, mkInstrList(mkInstr(I_LABEL, atom_name(l_true), atom_empty(), atom_empty(), atom_empty()), NULL)), append(body, mkInstrList(mkInstr(I_LABEL, atom_name(l_false), atom_empty(), atom_empty(), atom_empty()), NULL)));
			break;
		}

		case C_IF_ELSE:
		{
			char* l_true = newLabel();
			char* l_false = newLabel();
			char* l_endif = newLabel();
			InstrList* cond = compileBoolCond(l_true, l_false, cmd->attr.ifthenelse.cond);
			InstrList* body1 = compileCmd(cmd->attr.ifthenelse.cmd1);
			body1 = append(body1, mkInstrList(mkInstr(I_GOTO, atom_name(l_endif), atom_empty(), atom_empty(), atom_empty()), NULL));
			InstrList* body2 = compileCmd(cmd->attr.ifthenelse.cmd2);
			code = append(append(cond, append(append(mkInstrList(mkInstr(I_LABEL, atom_name(l_true), atom_empty(), atom_empty(), atom_empty()), NULL), body1), append(mkInstrList(mkInstr(I_LABEL, atom_name(l_false), atom_empty(), atom_empty(), atom_empty()), NULL), body2))), mkInstrList(mkInstr(I_LABEL, atom_name(l_endif), atom_empty(), atom_empty(), atom_empty()), NULL));
			break;
		}

		case C_WHILE:
		{
			char* l_top = newLabel();
			char* l_true = newLabel();
			char* l_false = newLabel();
			InstrList* cond = compileBoolCond(l_true, l_false, cmd->attr.whileloop.cond);
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

	// The next command can now reuse the registers used by the previous command
	var_counter_t = 0;
	var_counter_s = 0;
	var_counter_inf = 10;

	// Keeps compiling the next commands
	if(cmd->nextcmd != NULL)
		code = append(code, compileCmd(cmd->nextcmd));

	return code;
}