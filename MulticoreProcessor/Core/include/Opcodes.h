#ifndef OPCODES
#define OPCODES

/************************************
*      include                      *
************************************/
#include <stdint.h>
#include "../../Interface/include/Helpers.h"

/************************************
*      definitions                 *
************************************/
#define NUMBER_OF_OPCODES 21

/************************************
*       types                       *
************************************/
typedef enum
{
	ADD = 0,
	SUB,
	AND,
	OR,
	XOR,
	MUL,
	SLL,
	SRA,
	SRL,
	BEQ,
	BNE,
	BLT,
	BGT,
	BGE,
	JAL,
	LW,
	SW,
	HALT 
}OpcodeOperations;

typedef struct
{
	uint32_t *rd;
	uint32_t rs;
	uint32_t rt;
	uint32_t* memory_p;
	uint16_t *pc;			// PC is 10 bits.
	bool* halt;
}Opcode_fucntion_params_s;

typedef struct
{
	char* OpcodeName;
	char* OpcodeNumber;
	void (*OperationFunc)(Opcode_fucntion_params_s *params);
}Opcode;

Opcode OpcodeMapping[NUMBER_OF_OPCODES];


bool Opcode_IsBranchResulotion(uint16_t opcode);

#endif __OPCODES
