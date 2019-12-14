#ifndef __code_h__
#define __code_h__

typedef enum { I_ATRIB, I_PLUS, I_MINUS, I_DIV, I_MULT, I_MOD, I_LABEL, I_GOTO, I_IF, I_BEQ, I_BNE, I_BGT, I_BGE, I_BLT, I_BLE, I_OR, I_AND, I_PRINT, I_READ, I_LOAD, I_STORE } OpKind;

struct _Atom
{
	enum
	{ 
		NUMBER,
		STRING,
		EMPTY
	} kind;

	union
	{
		int value;
		char* name;
	} u;
};

typedef struct _Atom Atom;

struct _Instr
{
	OpKind op;
	Atom* el1;
	Atom* el2;
	Atom* el3;
	Atom* el4;
	struct _Instr* next;
};

typedef struct _Instr Instr;

struct _InstrList
{
	Instr* first;
	Instr* last;
};

typedef struct _InstrList InstrList;

Atom* atom_value(int value);
Atom* atom_name(char* name);
Atom* atom_empty();
Instr* mkInstr(OpKind op, Atom* el1, Atom* el2, Atom* el3, Atom* el4);
InstrList* mkInstrList(Instr* head, InstrList* tail);
InstrList* append(InstrList* l1, InstrList* l2);

void printAtom(Atom* a);
void printInstr(Instr* instr);
void printInstrList(InstrList* list);

InstrList* compileCmd(Cmd* cmd);

#endif