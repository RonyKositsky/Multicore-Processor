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
	*params->rd = params->rs + params->rt;
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
	*params->rd = params->rs - params->rt;
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
	*params->rd = params->rs & params->rt;
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
	*params->rd = params->rs | params->rt;
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
	*params->rd = params->rs ^ params->rt;
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
	*params->rd = params->rs * params->rt;
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
	*params->rd = params->rs << params->rt;
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
	*params->rd = (int)params->rs >> (int)params->rt;
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
	*params->rd = params->rs >> params->rt;
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
	if (params->rs == params->rt)
	{
		*params->pc = (uint16_t)(*params->rd & 0x1FF); // Taking the low 10 bits
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
	if (params->rs != params->rt)
	{
		*params->pc = (uint16_t)(*params->rd & 0x1FF); // Taking the low 10 bits
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
	if (params->rs < params->rt)
	{
		*params->pc = (uint16_t)(*params->rd & 0x1FF); // Taking the low 10 bits
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
	if (params->rs > params->rt)
	{
		*params->pc = (uint16_t)(*params->rd & 0x1FF); // Taking the low 10 bits
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
	if (params->rs <= params->rt)
	{
		*params->pc = (uint16_t)(*params->rd & 0x1FF); // Taking the low 10 bits
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
	if (params->rs >= params->rt)
	{
		*params->pc = (uint16_t)(*params->rd & 0x1FF); // Taking the low 10 bits
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
	*params->rd = *params->pc;
	*params->pc = (uint16_t)(*params->rd & 0x1FF); // Taking the low 10 bits
}

/************************************
*       API implementation          *
************************************/
bool Opcode_IsBranchResulotion(uint16_t opcode)
{
	return opcode >= BEQ && opcode < LW;
}

bool Opcode_IsMemoryCommand(uint16_t opcode)
{
	return opcode == LW || opcode == SW;
}


