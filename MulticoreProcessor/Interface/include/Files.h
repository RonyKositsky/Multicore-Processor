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

/************************************
*      variables                    *
************************************/
FILE* Imem0File;
FILE* Imem1File;
FILE* Imem2File;
FILE* Imem3File;
FILE* MeminFile;
FILE* MemoutFile;
FILE* Regout0File;
FILE* Regout1File;
FILE* Regout2File;
FILE* Regout3File;
FILE* Core0TraceFile;
FILE* Core1TraceFile;
FILE* Core2TraceFile;
FILE* Core3TraceFile;
FILE* BusTraceFile;
FILE* DsRam0TraceFile;
FILE* DsRam1TraceFile;
FILE* DsRam2TraceFile;
FILE* DsRam3TraceFile;
FILE* TsRam0TraceFile;
FILE* TsRam1TraceFile;
FILE* TsRam2TraceFile;
FILE* TsRam3TraceFile;
FILE* Stats0TraceFile;
FILE* Stats1TraceFile;
FILE* Stats2TraceFile;
FILE* Stats3TraceFile;

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


















