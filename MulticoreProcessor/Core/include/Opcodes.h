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
	BLE,
	BGE,
	JAL,
	LW,
	SW,
	HALT = 20 
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

void Add(Opcode_fucntion_params_s* params);
void Sub(Opcode_fucntion_params_s* params);
void And(Opcode_fucntion_params_s* params);
void Or(Opcode_fucntion_params_s* params);
void Xor(Opcode_fucntion_params_s* params);
void Multiply(Opcode_fucntion_params_s* params);
void LogicShiftLeft(Opcode_fucntion_params_s* params);
void ArithmeticShiftRight(Opcode_fucntion_params_s* params);
void LogicShiftRight(Opcode_fucntion_params_s* params);
void BranchEqual(Opcode_fucntion_params_s* params);
void BranchNotEqual(Opcode_fucntion_params_s* params);
void BranchLessThen(Opcode_fucntion_params_s* params);
void BranchGraterthen(Opcode_fucntion_params_s* params);
void BranchLessEqual(Opcode_fucntion_params_s* params);
void BranchGraterEqual(Opcode_fucntion_params_s* params);
void JumpAndLink(Opcode_fucntion_params_s* params);

static void (*OpcodeMapping[NUMBER_OF_OPCODES])(Opcode_fucntion_params_s* params) = {
	 Add, Sub, And, Or, Xor, Multiply, LogicShiftLeft, ArithmeticShiftRight,
	 LogicShiftRight, BranchEqual, BranchNotEqual, BranchLessThen, BranchGraterthen,
	 BranchLessEqual, BranchGraterEqual, JumpAndLink
};

/************************************
*				API                 *
************************************/

/*!
******************************************************************************
\brief
Returns wether the opcode is of baranch resulotion.

\param
 [in] uint16_t opcode - the opcode we are testing.
 [out] bool

\return true if thie is branch resulotion, false otherwise.
*****************************************************************************/
bool Opcode_IsBranchResulotion(uint16_t opcode);

/*!
******************************************************************************
\brief
Returns wether the opcode is of memory command.

\param
 [in] uint16_t opcode - the opcode we are testing.
 [out] bool

\return true if thie is memory command, false otherwise.
*****************************************************************************/
bool Opcode_IsMemoryCommand(uint16_t opcode);

#endif __OPCODES
