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
static void bubble_commands(Pipeline_s* pipeline);
static void execute_stage(Pipeline_s* pipeline);
static void prepare_registers_params(Opcode_fucntion_params_s* params, InstructionFormat_s command);
static void (*pipe_functions[PIPELINE_SIZE])(Pipeline_s* pipeline) =
{
	fetch, decode, execute, mem, writeback
};


/************************************
*       API implementation          *
************************************/
void Pipeline_Init(Pipeline_s *pipeline)
{
	memset(&pipeline->opcode_params, 0, sizeof(Opcode_fucntion_params_s));
	memset(pipeline->pipe_stages, 0, sizeof(PIPELINE_SIZE));
	pipeline->halted = false;

	for (int i = 0; i < PIPELINE_SIZE; i++)
	{
		pipeline->pipe_stages[i].is_init = false;
		pipeline->pipe_stages[i].is_stalled = false;
		pipeline->pipe_stages[i].state = i;
		pipeline->pipe_stages[i].pc = 0;
	}

}

void Pipeline_Execute(Pipeline_s* pipeline)
{
	stall_resulotion(pipeline);
	bubble_commands(pipeline);
	execute_stage(pipeline);
}

void Pipeline_WriteToTrace(Pipeline_s* pipeline, FILE* trace_file)
{
	for (int i = 0; i < PIPELINE_SIZE; i++)
	{
		if (!pipeline->pipe_stages[i].is_init || pipeline->pipe_stages[i].is_stalled)
			fprintf(trace_file, "--- ");
		else
			fprintf(trace_file, "%03X ", pipeline->pipe_stages[i].pc);
	}
}

/************************************
* static implementation             *
************************************/
static void fetch(Pipeline_s* pipeline)
{
	pipeline->pipe_stages[FETCH].pc = *(pipeline->opcode_params.pc);
	pipeline->fetched_operation.command = pipeline->insturcionts[*(pipeline->opcode_params.pc)];
}

static void decode(Pipeline_s* pipeline)
{
	pipeline->operation = OpcodeMapping[pipeline->fetched_operation.bits.opcode].OperationFunc;
}

static void execute(Pipeline_s* pipeline)
{
	pipeline->operation(&pipeline->opcode_params);
}

static void mem(Pipeline_s* pipeline)
{

}

static void writeback(Pipeline_s* pipeline)
{

}

static void stall_resulotion(Pipeline_s* pipeline)
{

}

static void prepare_registers_params(Opcode_fucntion_params_s* params, InstructionFormat_s command)
{
	params->rd		  = command.bits.rd;
	params->rs		  = command.bits.rd;
	params->rt		  = command.bits.rd;
	params->registers_p[IMMEDIATE_REGISTER_INDEX] = command.bits.immediate;
}

static void bubble_commands(Pipeline_s* pipeline)
{
	for (int i = PIPELINE_SIZE - 1; i > 0; i--)
	{
		if (pipeline->pipe_stages[i - 1].is_init)
		{
			pipeline->pipe_stages[i].is_init = true;
			pipeline->pipe_stages[i].pc = pipeline->pipe_stages[i - 1].pc;
		}
	}
	pipeline->pipe_stages[FETCH].is_init = true;
}

static void execute_stage(Pipeline_s* pipeline)
{
	for (int i = 0; i < PIPELINE_SIZE; i++)
	{
		if (pipeline->pipe_stages[i].is_init)
		{
			pipe_functions[i](pipeline);
		}
	}
}