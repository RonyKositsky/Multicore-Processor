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

#ifndef CORE
#define CORE

/************************************
*      include                      *
************************************/
#include "Helpers.h"
#include "Opcodes.h"

/************************************
*      definitions                 *
************************************/
#define NUMBER_OF_REGISTERS 16
#define MEMORY_SIZE 4096

/************************************
*       types                       *
************************************/
typedef struct
{
	uint ProgramCounter;
	uint InstructionCounter;
	uint RegisterArray[NUMBER_OF_REGISTERS];
	uint Memory[MEMORY_SIZE]; //Memoery image array.
}Core;

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
void InitCore();

/*!
******************************************************************************
\brief
Asks whether the core is halted or not.

\param
none

\return True if halted, false otherwise.
*****************************************************************************/
int CoreHalted();

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
InstructionCommand* GetInstructionCommand(uint pc);

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
void InstructionInit(void);

//??
void FreeInstructionsCommandArray(void);

#endif //CORE