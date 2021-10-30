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

typedef struct
{
	uint16_t ProgramCounter;	// pc is 10bit
	uint32_t InstructionCounter;
	uint32_t RegisterArray[NUMBER_OF_REGISTERS];
	uint32_t Memory[MEMORY_SIZE]; //Memoery image array.
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
none

\return none
*****************************************************************************/
void InitCore(Core_s *core);

/*!
******************************************************************************
\brief
Asks whether the core is halted or not.

\param
none

\return True if halted, false otherwise.
*****************************************************************************/
int CoreHalted(Core_s* core);

/*!
******************************************************************************
\brief
Init the instrucions command list.

\details
Called at the start of the run.

\param
none

\return none
*****************************************************************************/
//InstructionCommand* GetInstructionCommand(uint32_t pc);

/*!
******************************************************************************
\brief
Init the instrucions command list.

\details
Called at the start of the run.

\param
none

\return none
*****************************************************************************/
//void InstructionInit(void);

//??
//void FreeInstructionsCommandArray(void);

#endif //CORE_H_