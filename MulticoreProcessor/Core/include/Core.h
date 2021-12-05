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
#include <stdint.h>
#include "Pipeline.h"
#include "../../Interface/include/Helpers.h"
#include "../../Interface/include/Files.h"

/************************************
*      definitions                 *
************************************/
#define INSTRUCTIONS_MEMORY_SIZE 1024

/************************************
*       types                       *
************************************/
typedef struct 
{ 
	uint32_t cycles;
	uint32_t instructions;
}Statistics_s;

typedef struct
{
	bool core_halted;
	uint8_t index;
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
void Core_Init(Core_s* core, uint8_t id);

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
void Core_Iter(Core_s* core);

/*!
******************************************************************************
\brief
Teardown of the code.

\param
 [in] core - the operating core

\return none
*****************************************************************************/
void Core_Teaddown(Core_s* core);

/*!
******************************************************************************
\brief
If the core is halted.

\param
 [in] core - the operating core
 [out] bool

\return true if core is halted, fale otherwise.
*****************************************************************************/
bool Core_Halted(Core_s* core);

#endif //CORE_H_