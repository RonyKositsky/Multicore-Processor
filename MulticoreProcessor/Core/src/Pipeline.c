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
static bool compare_register(Pipeline_s* pipeline, uint16_t reg);
static void bubble_instruction(Pipeline_s* pipeline);
static void (*pipe_functions[PIPELINE_SIZE])(Pipeline_s* pipeline) =
{
	fetch, decode, execute, mem, writeback
};


/************************************
*       API implementation          *
************************************/
void Pipeline_Init(Pipeline_s *pipeline)
{
	pipeline->halted = false;
	pipeline->stalled = false;
	pipeline->reset_stall_flag = false;
	
	memset(&pipeline->opcode_params, 0, sizeof(Opcode_fucntion_params_s));
	pipeline->opcode_params.halt = &pipeline->halted;

	memset(pipeline->pipe_stages, 0, sizeof(PIPELINE_SIZE));
	for (int stage = FETCH; stage < PIPELINE_SIZE; stage++)
	{
		pipeline->pipe_stages[stage].is_init =  false;
		pipeline->pipe_stages[stage].is_stalled =  false;
		pipeline->pipe_stages[stage].state = stage;
		pipeline->pipe_stages[stage].pc = 0;
	}
}

void Pipeline_Execute(Pipeline_s* pipeline)
{
	check_initialization(pipeline);
	execute_stages(pipeline);
	if (pipeline->stalled && !pipeline_needs_stall(pipeline))
	{
		free_stall_flags(pipeline);
	}
}

void Pipeline_WriteToTrace(Pipeline_s* pipeline, FILE* trace_file)
{
	for (int stage = FETCH; stage < PIPELINE_SIZE; stage++)
	{
		if (!pipeline->pipe_stages[stage].is_init)
			fprintf(trace_file, "--- ");
		else
			fprintf(trace_file, "%03X ", pipeline->pipe_stages[stage].pc);
	}

	bubble_instruction(pipeline);
}

/************************************
* static implementation             *
************************************/
static void fetch(Pipeline_s* pipeline)
{
	pipeline->pipe_stages[FETCH].pc = *(pipeline->opcode_params.pc);
	pipeline->pipe_stages[FETCH].instruction.command = pipeline->insturcionts_p[*(pipeline->opcode_params.pc)];
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
	//Update registers
	for (int reg = STRART_MUTABLE_REGISTER_INDEX; reg < NUMBER_OF_REGISTERS; reg++)
	{
		pipeline->core_registers_p[reg] = pipeline->opcode_params.registers[reg];
	}
}


static void prepare_registers_params(Opcode_fucntion_params_s* params, InstructionFormat_s command)
{
	params->rd	= command.bits.rd;
	params->rs	= command.bits.rs;
	params->rt	= command.bits.rt;
	params->registers[IMMEDIATE_REGISTER_INDEX] = command.bits.immediate;
}

static bool deinit_stage(Pipeline_s *pipeline, PipelineSM_s stage)
{
	return (pipeline->pipe_stages[stage - 1].is_stalled) || !(pipeline->pipe_stages[stage-1].is_init);
}

static void check_initialization(Pipeline_s* pipeline)
{
	// Avoid situation in which we init the stage and deinit it right after.
	static int initialized_stage = DECODE;

	if (!pipeline->pipe_stages[FETCH].is_init)
	{
		pipeline->pipe_stages[FETCH].is_init = true;
	}
	else 
	{
		for (int stage = PIPELINE_SIZE - 1; stage > 0; stage--)
		{
			if (pipeline->pipe_stages[stage - 1].is_init)
			{
				pipeline->pipe_stages[stage].is_init = true;
				initialized_stage = stage;
				break;
			}
		}
	}

	for(int stage = PIPELINE_SIZE - 1; stage > DECODE;  stage--)
	{
		if (deinit_stage(pipeline, stage) && stage != initialized_stage)
		{
			pipeline->pipe_stages[stage].is_init = false;
		}
	}
}

static void bubble_instruction(Pipeline_s* pipeline)
{
	for(int stage = PIPELINE_SIZE -1 ; stage > FETCH; stage--)
	{
		if (pipeline->pipe_stages[stage - 1].is_stalled)
			break;

		pipeline->pipe_stages[stage].pc = pipeline->pipe_stages[stage - 1].pc;
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
	return compare_register(pipeline, pipeline->pipe_stages[stage].instruction.bits.rd);
}

static bool pipeline_needs_stall(Pipeline_s* pipeline)
{
	return find_registers_equality(pipeline, EXECUTE) || find_registers_equality(pipeline, MEM) 
		|| find_registers_equality(pipeline, WRITE_BACK);
}

static void init_stall_flags(Pipeline_s* pipeline)
{
	pipeline->stalled = true;
	pipeline->pipe_stages[FETCH].is_stalled = true;
	pipeline->pipe_stages[DECODE].is_stalled = true;
}

static void free_stall_flags(Pipeline_s* pipeline)
{
	pipeline->reset_stall_flag = true;
	pipeline->pipe_stages[FETCH].is_stalled = false;
	pipeline->pipe_stages[DECODE].is_stalled = false;
}