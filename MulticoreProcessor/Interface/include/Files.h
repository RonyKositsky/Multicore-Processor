/*!
******************************************************************************
\file Bus.h
\date 17 October 2021
\author Rony Kosistky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan Gartenberg
\par
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef FILES_H_
#define FILES_H_

/************************************
*      include                      *
************************************/
#include <stdio.h>
#include "Helpers.h"

/************************************
*      variables                    *
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

static Core_Files CoresFilesArray[NUMBER_OF_CORES];
FILE* MeminFile;
FILE* MemoutFile;
FILE* BusTraceFile;

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initialize all the files from the user input.

\details
Called at the start of the program.

\param
 [in] argv[]   - user input
 [out] out_val - True if all files are valid, False otherwise/

\return none
*****************************************************************************/
int OpenFiles(char* argv[]);

/*!
******************************************************************************
\brief
Closing all the files.

\details
Called at the end of the run.

\param
 [in]  none
 [out] none

\return none
*****************************************************************************/
void CloseFiles(void);

#endif //__FILE_NAME_H__


















