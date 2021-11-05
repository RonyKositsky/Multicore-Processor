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
#include "Pipeline.h"
#include "../../Interface/include/Helpers.h"

/************************************
*      definitions                 *
************************************/
#define MEMORY_SIZE 1024

/************************************
*       types                       *
************************************/
typedef struct 
{
	FILE* InstructionMemFile;
	FILE* RegFile;
	FILE* TraceFile;
	FILE* DsRamFile;
	FILE* TsRamFile;
	FILE* StatsFile;
}Core_Files;

typedef struct
{
	uint16_t ProgramCounter;						// pc is 10bit
	uint32_t InstructionCounter;
	uint32_t RegisterArray[NUMBER_OF_REGISTERS];
	uint32_t InstructionsMemory[MEMORY_SIZE];		//Instructions memory.
	InstructionFormat_s InstructionCommand;
	Core_Files Files;
	Pipeline_s pipeline;
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