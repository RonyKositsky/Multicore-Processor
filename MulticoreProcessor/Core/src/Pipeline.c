/*!
******************************************************************************
\file Pipeline.c
\date 17 October 2021
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
#include "..\include\Pipeline.h"

/************************************
*      definitions                 *
************************************/

/************************************
*       types                       *
************************************/

/************************************
*      variables                    *
************************************/

/************************************
*      static functions             *
************************************/
static void stall_resulotion(Pipeline_s* pipeline);
static void fetch(Pipeline_s* pipeline);
static void decode(Pipeline_s* pipeline);
static void execute(Pipeline_s* pipeline);
static void mem(Pipeline_s* pipeline);
static void writeback(Pipeline_s* pipeline);
static void (*pipe_functions[PIPELINE_SIZE])(Pipeline_s* pipeline) =
{
	fetch, decode, execute, mem, writeback
};


/************************************
*       API implementation          *
************************************/
void Pipeline_Init(Pipeline_s *pipeline)
{
	pipeline -> in_stall = false;
	for (int i = 0; i < PIPELINE_SIZE; i++)
	{
		//do something
	}
}


void Pipeline_Execute(Pipeline_s* pipeline, uint16_t pc)
{
	stall_resulotion(pipeline);
	for (int i = 0; i < PIPELINE_SIZE; i++)
	{
		pipe_functions[i](pipeline);
	}
}


/************************************
* static implementation             *
************************************/
static void fetch(Pipeline_s* pipeline)
{
	//pipeline -> fetched_operation = pipeline->insturcionts[pipeline->opcode_params.pc];
}

static void decode(Pipeline_s* pipeline)
{
	pipeline->operation = OpcodeMapping[pipeline->fetched_operation].OperationFunc;
}

static void execute(Pipeline_s* pipeline)
{
	pipeline->operation(pipeline->opcode_params);
}

static void mem(Pipeline_s* pipeline)
{

}

static void writeback(Pipeline_s* pipeline)
{

}

static void stall_resulotion(Pipeline_s* pipeline)
{
	pipeline->in_stall = false;
}
