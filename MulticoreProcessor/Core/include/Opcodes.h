﻿#ifndef OPCODES
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
	uint16_t *pc;			// PC is 10 bits.
	uint16_t immediate;		// Immediate is 12 bits.
}Opcode_fucntion_params_s;

typedef struct
{
	char* OpcodeName;
	char* OpcodeNumber;
	void (*OperationFunc)(Opcode_fucntion_params_s *params);
}Opcode;

Opcode OpcodeMapping[NUMBER_OF_OPCODES];

#endif __OPCODES
