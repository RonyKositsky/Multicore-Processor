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
static bool pipeline_needs_stall(Pipeline_s* pipeline);
static void fetch(Pipeline_s* pipeline);
static void decode(Pipeline_s* pipeline);
static void execute(Pipeline_s* pipeline);
static void mem(Pipeline_s* pipeline);
static void writeback(Pipeline_s* pipeline);
static void check_initialization(Pipeline_s* pipeline);
static void execute_stages(Pipeline_s* pipeline);
static void prepare_registers_params(Opcode_fucntion_params_s* params, InstructionFormat_s command);
static bool find_registers_equality(Pipeline_s* pipeline, PipelineSM_s stage);
static void init_stall_flags(Pipeline_s* pipeline);
static void free_stall_flags(Pipeline_s* pipeline);
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

	for (int stage = FETCH; stage < PIPELINE_SIZE; stage++)
	{
		pipeline->pipe_stages[stage].is_init = stage == FETCH ? true : false;
		pipeline->pipe_stages[stage].is_stalled =  false;
		pipeline->pipe_stages[stage].state = stage;
		pipeline->pipe_stages[stage].pc = 0;
	}
}

void Pipeline_Execute(Pipeline_s* pipeline)
{
	execute_stages(pipeline);
	check_initialization(pipeline);
	if (!pipeline_needs_stall(pipeline) && pipeline->stalled)
	{
		free_stall_flags(pipeline);
	}
}

void Pipeline_WriteToTrace(Pipeline_s* pipeline, FILE* trace_file)
{
	for (int stage = FETCH; stage < PIPELINE_SIZE; stage++)
	{
		if (!pipeline->pipe_stages[stage].is_init || pipeline->pipe_stages[stage].is_stalled)
			fprintf(trace_file, "--- ");
		else
			fprintf(trace_file, "%03X ", pipeline->pipe_stages[stage].pc);
	}
}

/************************************
* static implementation             *
************************************/
static void fetch(Pipeline_s* pipeline)
{
	pipeline->pipe_stages[FETCH].pc = *(pipeline->opcode_params.pc);
	pipeline->pipe_stages[FETCH].instruction.command = pipeline->insturcionts[*(pipeline->opcode_params.pc)];
}

static void decode(Pipeline_s* pipeline)
{
	pipeline->operation = OpcodeMapping[pipeline->pipe_stages[DECODE].instruction.bits.opcode].OperationFunc;
	if (pipeline_needs_stall(pipeline))
	{
		init_stall_flags(pipeline);
	}
}

static void execute(Pipeline_s* pipeline)
{
	prepare_registers_params(&pipeline->opcode_params, pipeline->pipe_stages[EXECUTE].instruction);
	pipeline->operation(&pipeline->opcode_params);
}

static void mem(Pipeline_s* pipeline)
{

}

static void writeback(Pipeline_s* pipeline)
{

}


static void prepare_registers_params(Opcode_fucntion_params_s* params, InstructionFormat_s command)
{
	params->rd	= command.bits.rd;
	params->rs	= command.bits.rd;
	params->rt	= command.bits.rd;
	params->registers_p[IMMEDIATE_REGISTER_INDEX] = command.bits.immediate;
}

static bool deinit_stage(Pipeline_s *pipeline, PipelineSM_s stage)
{
	return (pipeline->pipe_stages[stage - 1].is_stalled) || (pipeline->pipe_stages[stage-1].is_init);
}

static void check_initialization(Pipeline_s* pipeline)
{
	// Avoid situation in which we init the stage and deinit it right after.
	static int initialized_stage = DECODE;

	for (int stage = PIPELINE_SIZE - 1; stage > 0; stage--)
	{
		if (pipeline->pipe_stages[stage - 1].is_init)
		{
			pipeline->pipe_stages[stage].is_init = true;
			initialized_stage = stage;
			break;
		}
	}

	if (!pipeline->stalled)
	{
		return;
	}

	for(int stage = PIPELINE_SIZE - 1; stage > DECODE;  stage--)
	{
		if(deinit_stage(pipeline, stage) && stage != initialized_stage)
		{
			pipeline->pipe_stages[stage].is_init = false;
		}
	}
}

static void bubble_instruction(Pipeline_s* pipeline, PipelineSM_s stage)
{
	pipeline->pipe_stages[stage].pc = pipeline->pipe_stages[stage - 1].pc;
	if (stage != FETCH)
	{
		pipeline->pipe_stages[stage].instruction.command =
			pipeline->pipe_stages[stage - 1].instruction.command;
	}
}

static void execute_stages(Pipeline_s* pipeline)
{
	for (int stage = 0; stage < PIPELINE_SIZE; stage++)
	{
		if (pipeline->pipe_stages[stage].is_init && !pipeline->pipe_stages[stage].is_stalled)
		{
			bubble_instruction(pipeline, stage);
			pipe_functions[stage](pipeline);
		}
	}
}

static bool compare_register(Pipeline_s* pipeline, uint16_t reg)
{
	if (reg == IMMEDIATE_REGISTER_INDEX || reg == ZERO_REGISTER_INDEX)
	{
		return false;
	}

	InstructionFormat_s decode_ins = pipeline->pipe_stages[DECODE].instruction;
	return reg == decode_ins.bits.rd || reg == decode_ins.bits.rs || reg == decode_ins.bits.rt;
}

static bool find_registers_equality(Pipeline_s *pipeline, PipelineSM_s stage)
{
	if (!pipeline->pipe_stages[stage].is_init)
	{
		return false;
	}
	InstructionFormat_s tested_ins = pipeline->pipe_stages[stage].instruction;
	return compare_register(pipeline, tested_ins.bits.rd) || compare_register(pipeline, tested_ins.bits.rs) ||
		compare_register(pipeline, tested_ins.bits.rt);
}

static bool pipeline_needs_stall(Pipeline_s* pipeline)
{
	return find_registers_equality(pipeline, EXECUTE) || find_registers_equality(pipeline, MEM) ||
		find_registers_equality(pipeline, WRITE_BACK);
}

static void init_stall_flags(Pipeline_s* pipeline)
{
	pipeline->stalled = false;
	pipeline->pipe_stages[FETCH].is_stalled = false;
	pipeline->pipe_stages[DECODE].is_stalled = false;
}

static void free_stall_flags(Pipeline_s* pipeline)
{
	pipeline->stalled = false;
	pipeline->pipe_stages[FETCH].is_stalled = false;
	pipeline->pipe_stages[DECODE].is_stalled = false;
}