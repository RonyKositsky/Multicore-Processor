#ifndef OPCODES
#define OPCODES

/************************************
*      include                      *
************************************/
#include <stdint.h>
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
	void (*OperationFunc)(uint32_t rd, uint32_t rs, uint32_t rt, Core_s *core);
}Opcode;

typedef struct
{
	uint32_t PCLocation;
	Opcode opcode;
	uint32_t rd;
	uint32_t rs;
	uint32_t rt;
	int ImmValue;
	int HasImmediate;
	char* Name;
}InstructionCommand;

Opcode OpcodeMapping[NUMBER_OF_OPCODES];

#endif __OPCODES
