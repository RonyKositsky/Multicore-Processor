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
#define INSTRUCTIONS_MEMORY_SIZE 1024

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
	uint32_t cycles;
	uint32_t instructions;
	uint32_t read_hits;
	uint32_t write_hits;
	uint32_t read_misses;
	uint32_t write_misses;
	uint32_t decode_stalls;
	uint32_t mem_stalls;
}Statistics_s;

typedef struct
{
	uint16_t program_counter;	// pc is 10bit
	uint32_t register_array[NUMBER_OF_REGISTERS];
	uint32_t instructions_memory_image[INSTRUCTIONS_MEMORY_SIZE];				
	Core_Files core_files;
	Pipeline_s pipeline;
	Statistics_s statistics;
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