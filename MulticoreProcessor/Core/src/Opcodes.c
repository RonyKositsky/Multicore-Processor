/*!
******************************************************************************
\file
\date 26 October 2021
\author Rony Kositsky & Ofir Guthman & Yonatan Gartenberg
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
#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_COUNTER_REGISTER_NUM 15

/************************************
*      static functions             *
************************************/

/*!
******************************************************************************
\brief
R[rd] = R[rs] + R[rt]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Add(uint32_t rd, uint32_t rs, uint32_t rt, Core_s *core)
{
	core->RegisterArray[rd] = core->RegisterArray[rs] + core->RegisterArray[rt];
}

/*!
******************************************************************************
\brief
R[rd] = R[rs] - R[rt]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Sub(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core)
{
	core->RegisterArray[rd] = core->RegisterArray[rs] - core->RegisterArray[rt];
}

/*!
******************************************************************************
\brief
R[rd] = R[rs] & R[rt]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void And(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core)
{
	core->RegisterArray[rd] = core->RegisterArray[rs] & core->RegisterArray[rt];
}

/*!
******************************************************************************
\brief
R[rd] = R[rs] | R[rt]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Or(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core)
{
	core->RegisterArray[rd] = core->RegisterArray[rs] | core->RegisterArray[rt];
}

/*!
******************************************************************************
\brief
R[rd] = R[rs] ^ R[rt]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Xor(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core)
{
	core->RegisterArray[rd] = core->RegisterArray[rs] ^ core->RegisterArray[rt];
}

/*!
******************************************************************************
\brief
R[rd] = R[rs] * R[rt]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Multiply(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core)
{
	core->RegisterArray[rd] = core->RegisterArray[rs] * core->RegisterArray[rt];
}

/*!
******************************************************************************
\brief
R[rd] = R[rs] << R[rt]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void LogicShiftLeft(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core)
{
	core->RegisterArray[rd] = core->RegisterArray[rs] << core->RegisterArray[rt];
}

/*!
******************************************************************************
\brief
R[rd] = R[rs] >> R[rt] (arithmetic shift with sign extension)

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void ArithmeticShiftRight(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core)
{
	core->RegisterArray[rd] = (int)core->RegisterArray[rs] >> (int)core->RegisterArray[rt];
} 

/*!
******************************************************************************
\brief
R[rd] = R[rs] >> R[rt]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void LogicShiftRight(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core)
{
	core->RegisterArray[rd] = core->RegisterArray[rs] >> core->RegisterArray[rt];
}

/*!
******************************************************************************
\brief
if (R[rs] == R[rt]) pc = R[rd][low bits 9:0]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchEqual(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core) 
{
	if (core->RegisterArray[rs] == core->RegisterArray[rt])
		core->ProgramCounter = core->RegisterArray[rd] & 0x1FF; // Taking the low 10 bits
}

/*!
******************************************************************************
\brief
if (R[rs] != R[rt]) pc = R[rd][low bits 9:0]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchNotEqual(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core) 
{
	if (core->RegisterArray[rs] != core->RegisterArray[rt])
		core->ProgramCounter = core->RegisterArray[rd] & 0x1FF; // Taking the low 10 bits
}

/*!
******************************************************************************
\brief
if (R[rs] < R[rt]) pc = R[rd][low bits 9:0]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchLessThen(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core) 
{
	if (core->RegisterArray[rs] < core->RegisterArray[rt])
		core->ProgramCounter = core->RegisterArray[rd] & 0x1FF; // Taking the low 10 bits
}

/*!
******************************************************************************
\brief
if (R[rs] > R[rt]) pc = R[rd][low bits 9:0]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchGraterThen(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core) 
{
	if (core->RegisterArray[rs] > core->RegisterArray[rt])
		core->ProgramCounter = core->RegisterArray[rd] & 0x1FF; // Taking the low 10 bits
}

/*!
******************************************************************************
\brief
if (R[rs] <= R[rt]) pc = R[rd][low bits 9:0]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchLessEqual(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core) 
{
	if (core->RegisterArray[rs] <= core->RegisterArray[rt])
		core->ProgramCounter = core->RegisterArray[rd] & 0x1FF; // Taking the low 10 bits
}

/*!
******************************************************************************
\brief
if (R[rs] >= R[rt]) pc = R[rd][low bits 9:0]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void BranchGraterEqual(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core) 
{
	if (core->RegisterArray[rs] >= core->RegisterArray[rt])
		core->ProgramCounter = core->RegisterArray[rd] & 0x1FF; // Taking the low 10 bits
}

/*!
******************************************************************************
\brief
R[15] = next instruction address, pc = R[rd][9:0]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void JumpAndLink(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core) {
	core->RegisterArray[PROGRAM_COUNTER_REGISTER_NUM] = core->ProgramCounter;
	core->ProgramCounter = core->RegisterArray[rd] & 0x1FF;
}

/*!
******************************************************************************
\brief
R[rd] = MEM[R[rs]+R[rt]]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void LoadWord(uint32_t rd, uint32_t rs, uint32_t rt, Core_s *core) {
	uint32_t address = core->RegisterArray[rs] + core->RegisterArray[rt];
	core -> RegisterArray[rd] = core -> Memory[address];
}

/*!
******************************************************************************
\brief
MEM[R[rs]+R[rt]] = R[rd]

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void StoreWord(uint32_t rd, uint32_t rs, uint32_t rt, Core_s* core) {
	uint32_t address = core->RegisterArray[rs] + core->RegisterArray[rt];
	core -> Memory[address] = core->RegisterArray[rd];
}

/*!
******************************************************************************
\brief
Halt this core

\param
[in] rd - rd register value.
[in] rs - rs register value.
[in] rt - rt register value.
[in] core - The core on which we will run the operation.

\return none
*****************************************************************************/
void Halt(uint32_t rd, uint32_t rs, uint32_t rt, Core_s *core)
{
	//core->ProgramCounter = INSTRUCTION_COUNT + 1;
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
	{"bgt", "0C", BranchGraterThen},
	{"ble", "0D", BranchLessEqual},
	{"bge", "0E", BranchGraterEqual},
	{"jal", "0F", JumpAndLink},
	{"lw", "10", LoadWord},
	{"sw", "11", StoreWord},
	{"halt", "12", Halt},
};


