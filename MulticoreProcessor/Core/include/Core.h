/*!
******************************************************************************
\file file_name.h
\date 26 October 2021
\author Rony Kosistky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan Gartenberg
\par
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef CORE_H_
#define CORE_H_

/************************************
*      include                      *
************************************/
#include <stdio.h>
#include <stdint.h>

/************************************
*      definitions                 *
************************************/
#define NUMBER_OF_REGISTERS 16
#define MEMORY_SIZE 1024
#define NUMBER_OF_CORES 4

/************************************
*       types                       *
************************************/
typedef struct 
{
	FILE* MemFile;
	FILE* RegFile;
	FILE* TraceFile;
	FILE* DsRamFile;
	FILE* TsRamFile;
	FILE* StatsFile;
}Core_Files;

typedef union
{
	struct
	{
		uint16_t immediate : 12;	// [0:11]  Immediate value
		uint16_t source_1 : 4;		// [12:15] src1 value
		uint16_t source_0 : 4;		// [16:19] src0 value
		uint16_t destination : 4;	// [20:23] src0 value
		uint16_t opcode : 8;		// [24:31] opcode value
	} bits;

	uint32_t command;
} InstructionFormat_s;

typedef struct
{
	uint16_t ProgramCounter;						// pc is 10bit
	uint32_t InstructionCounter;
	uint32_t RegisterArray[NUMBER_OF_REGISTERS];
	uint32_t Memory[MEMORY_SIZE];					//Memoery image array.
	InstructionFormat_s InstructionCommand;
	Core_Files Files;
}Core_s;

/************************************
*       API                       *
************************************/

/*!
******************************************************************************
\brief
Init the core.

\details
Called at the start of the run.

\param
 [in] core - the operating core

\return none
*****************************************************************************/
void InitCore(Core_s* core);

/*!
******************************************************************************
\brief
Asks whether the core is halted or not.

\param
 [in] core - the operating core

\return True if halted, false otherwise.
*****************************************************************************/
int CoreHalted(Core_s* core);

/*!
******************************************************************************
\brief
Run core iteration

\details
core is running with pipeline.

\param
 [in] core - the operating core

\return none
*****************************************************************************/
void CoreIter(Core_s* core);

#endif //CORE_H_