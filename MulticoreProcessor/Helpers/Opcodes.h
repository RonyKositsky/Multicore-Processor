#ifndef OPCODES
#define OPCODES

/************************************
*      include                      *
************************************/
#include "Helpers.h"
#include "Core.h"

/************************************
*      definitions                 *
************************************/
#define NUMBER_OF_OPCODES 21

/************************************
*       types                       *
************************************/
typedef struct
{
	char* OpcodeName;
	char* OpcodeNumber;
	void (*OperationFunc)(uint rd, uint rs, uint rt, Core *core);
}Opcode;

typedef struct
{
	uint PCLocation;
	Opcode opcode;
	uint rd;
	uint rs;
	uint rt;
	int ImmValue;
	int HasImmediate;
	char* Name;
}InstructionCommand;

Opcode OpcodeMapping[NUMBER_OF_OPCODES];
#endif
