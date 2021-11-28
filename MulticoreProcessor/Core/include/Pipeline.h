/*!
******************************************************************************
\file Pipeline.h
\date 30 October 2021
\author Rony Kosistky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan Gartenberg
\par
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef __PIPELINE_H__
#define __PIPELINE_H__

/************************************
*      include                      *
************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "Cache.h"
#include "../../Interface/include/Helpers.h"
#include "Opcodes.h"

/************************************
*      definitions                 *
************************************/

/************************************
*       types                       *
************************************/
typedef enum 
{
	FETCH = 0,
	DECODE,
	EXECUTE,
	MEM,
	WRITE_BACK,

	PIPELINE_SIZE
} PipelineSM_e;

typedef struct
{
	bool is_stalled;
	PipelineSM_e state;
	uint16_t pc;
	InstructionFormat_s instruction;
	uint32_t execute_result;
	void (*operation)(Opcode_fucntion_params_s* params);
} PipelineStage_s;

typedef struct
{
	bool halted;
	bool stalled;
	uint32_t *insturcionts_p;
	uint32_t* core_registers_p;
	CacheData_s cache_data;
	PipelineStage_s pipe_stages[PIPELINE_SIZE];
	Opcode_fucntion_params_s opcode_params;
}Pipeline_s;


/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Init the pipeline.

\param
 [in]  none
 [out] none

\return none
*****************************************************************************/
void Pipeline_Init(Pipeline_s *pipeline);

/*!
******************************************************************************
\brief
One iteration of the pipeline. We will bubble the values inside the pipeline
based on it's condition.

\param
 [in]  none
 [out] none

\return none
*****************************************************************************/
void Pipeline_Execute(Pipeline_s* pipeline);

/*!
******************************************************************************
\brief
Writing the pipeline to the trace file.

\param
 [in]  none
 [out] none

\return none
*****************************************************************************/
void Pipeline_WriteToTrace(Pipeline_s* pipeline, FILE *trace_file);


/*!
******************************************************************************
\brief
Init the pipeline.

\param
 [in]  none
 [out] none

\return none
*****************************************************************************/
void Pipeline_BubbleCommands(Pipeline_s* pipeline);

#endif //__PIPELINE_H__