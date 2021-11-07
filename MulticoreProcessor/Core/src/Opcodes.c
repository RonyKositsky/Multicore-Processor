/*!
******************************************************************************
\file
\date 26 October 2021
\author Rony Kositsky & Ofir Guthman & Yonatan Gaparams->rtenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan
\par
ALL RIGHTS RESERVED
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "../include/Opcodes.h"

/************************************
*      definitions                 *
************************************/
#define _CSECURE_NO_WARNINGS
#define PROGRAM_COUNTER_REGISTER_NUM 15

/************************************
*      static functions             *
************************************/

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] + R[params->rt]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Add(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = params->registers[params->rs] + params->registers[params->rt];
}

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] - R[params->rt]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Sub(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = params->registers[params->rs] - params->registers[params->rt];
}

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] & R[params->rt]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void And(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = params->registers[params->rs] & params->registers[params->rt];
}

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] | R[params->rt]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Or(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = params->registers[params->rs] | params->registers[params->rt];
}

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] ^ R[params->rt]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Xor(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = params->registers[params->rs] ^ params->registers[params->rt];
}

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] * R[params->rt]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Multiply(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = params->registers[params->rs] * params->registers[params->rt];
}

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] << R[params->rt]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void LogicShiftLeft(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = params->registers[params->rs] << params->registers[params->rt];
}

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] >> R[params->rt] (arithmetic shift with sign extension)

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void ArithmeticShiftRight(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = (int)params->registers[params->rs] >> (int)params->registers[params->rt];
} 

/*!
******************************************************************************
\brief
R[params->rd] = R[params->rs] >> R[params->rt]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void LogicShiftRight(Opcode_fucntion_params_s *params)
{
	params->registers[params->rd] = params->registers[params->rs] >> params->registers[params->rt];
}

/*!
******************************************************************************
\brief
if (R[params->rs] == R[params->rt]) pc = R[params->rd][low bits 9:0]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchEqual(Opcode_fucntion_params_s *params)
{
	if (params->registers[params->rs] == params->registers[params->rt])
	{
		params->pc = params->registers[params->rd] & 0x1FF; // Taking the low 10 bits
	}
}

/*!
******************************************************************************
\brief
if (R[params->rs] != R[params->rt]) pc = R[params->rd][low bits 9:0]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchNotEqual(Opcode_fucntion_params_s* params)
{
	if (params->registers[params->rs] != params->registers[params->rt])
	{
		params->pc = params->registers[params->rd] & 0x1FF; // Taking the low 10 bits
	}
}

/*!
******************************************************************************
\brief
if (R[params->rs] < R[params->rt]) pc = R[params->rd][low bits 9:0]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchLessThen(Opcode_fucntion_params_s* params)
{
	if (params->registers[params->rs] < params->registers[params->rt])
	{
		*params->pc = params->registers[params->rd] & 0x1FF; // Taking the low 10 bits
	}
}

/*!
******************************************************************************
\brief
if (R[params->rs] > R[params->rt]) pc = R[params->rd][low bits 9:0]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchGraterthen(Opcode_fucntion_params_s* params)
{
	if (params->registers[params->rs] > params->registers[params->rt])
	{
		params->pc = params->registers[params->rd] & 0x1FF; // Taking the low 10 bits
	}
}

/*!
******************************************************************************
\brief
if (R[params->rs] <= R[params->rt]) pc = R[params->rd][low bits 9:0]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchLessEqual(Opcode_fucntion_params_s *params)
{
	if (params->registers[params->rs] <= params->registers[params->rt])
	{
		params->pc = params->registers[params->rd] & 0x1FF; // Taking the low 10 bits
	}
}

/*!
******************************************************************************
\brief
if (R[params->rs] >= R[params->rt]) pc = R[params->rd][low bits 9:0]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchGraterEqual(Opcode_fucntion_params_s *params)
{
	if (params->registers[params->rs] >= params->registers[params->rt])
	{
		params->pc = params->registers[params->rd] & 0x1FF; // Taking the low 10 bits
	}
}

/*!
******************************************************************************
\brief
R[15] = next instruction address, pc = R[params->rd][9:0]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void JumpAndLink(Opcode_fucntion_params_s *params) 
{
	params->registers[PROGRAM_COUNTER_REGISTER_NUM] = params->pc;
	params->pc = params->registers[params->rd] & 0x1FF;
}

/*!
******************************************************************************
\brief
R[params->rd] = MEM[R[params->rs]+R[params->rt]]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void LoadWord(Opcode_fucntion_params_s *params) 
{
	uint32_t address = params->registers[params->rs] + params->registers[params->rt];
	params->registers[params->rd] = params->memory_p[address];
}

/*!
******************************************************************************
\brief
MEM[R[params->rs]+R[params->rt]] = R[params->rd]

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void StoreWord(Opcode_fucntion_params_s *params)
{
	uint32_t address = params->registers[params->rs] + params->registers[params->rt];
	params->memory_p[address] = params->registers[params->rd];
}

/*!
******************************************************************************
\brief
Halt this core

\param
[in] params->rd - params->rd register value.
[in] params->rs - params->rs register value.
[in] params->rt - params->rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Halt(Opcode_fucntion_params_s *params)
{
	*params->halt = true;
}

/************************************
*       API implementation          *
************************************/
Opcode OpcodeMapping[NUMBER_OF_OPCODES] = {
	{"add", "00", Add},
	{"sub", "01", Sub},
	{"and", "02", And},
	{"or", "03", Or},
	{"xor", "04", Xor},
	{"mul", "05", Multiply},
	{"sll", "06", LogicShiftLeft},
	{"sra", "07", ArithmeticShiftRight},
	{"srl", "08", LogicShiftRight},
	{"beq", "09", BranchEqual},
	{"bne", "0A", BranchNotEqual},
	{"blt", "0B", BranchLessThen},
	{"bgt", "0C", BranchGraterthen},
	{"ble", "0D", BranchLessEqual},
	{"bge", "0E", BranchGraterEqual},
	{"jal", "0F", JumpAndLink},
	{"lw", "10", LoadWord},
	{"sw", "11", StoreWord},
	{"halt", "12", Halt},
};


