#ifndef OPCODES
#define OPCODES

/************************************
*      include                      *
************************************/
#include <stdint.h>

/************************************
*      definitions                 *
************************************/
#define NUMBER_OF_OPCODES 21

/************************************
*       types                       *
************************************/

typedef struct
{
	uint32_t rd;
	uint32_t rs;
	uint32_t rt;
	uint32_t *registers_p;
	uint32_t *memory_p;
	uint16_t pc;
}Opcode_fucntion_params_s;

typedef struct
{
	char* OpcodeName;
	char* OpcodeNumber;
	void (*OperationFunc)(Opcode_fucntion_params_s params);
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
