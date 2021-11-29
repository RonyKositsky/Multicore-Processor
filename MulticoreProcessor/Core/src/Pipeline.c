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
#include <string.h>
#include "..\include\Cache.h"

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
static void fetch(Pipeline_s* pipeline);
static void decode(Pipeline_s* pipeline);
static void execute(Pipeline_s* pipeline);
static void mem(Pipeline_s* pipeline);
static void writeback(Pipeline_s* pipeline);
static void execute_stages(Pipeline_s* pipeline);
static void prepare_registers_params(Pipeline_s* pipeline, PipelineSM_e stage);
static void init_data_hazard_stall(Pipeline_s* pipeline);
static void free_data_hazard_stall(Pipeline_s* pipeline);
static void handle_mem_stall(Pipeline_s* pipeline, bool stall);
static bool pipe_stalled(Pipeline_s* pipeline);
static bool pipeline_needs_data_hazard_stall(Pipeline_s* pipeline);
static bool compare_register(Pipeline_s* pipeline, uint16_t reg);
static bool check_registers_hazrads(Pipeline_s* pipeline, PipelineSM_e stage);
static bool can_free_data_hazard_stall(Pipeline_s* pipeline);
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
	pipeline->data_hazard_stall = false;
	pipeline->memory_stall = false;
	
	memset((uint8_t *) &pipeline->opcode_params, 0, sizeof(pipeline->opcode_params));
	pipeline->opcode_params.halt = &pipeline->halted;

	memset((uint8_t*) pipeline->pipe_stages, 0, sizeof(pipeline->pipe_stages));
	for (int stage = FETCH; stage < PIPELINE_SIZE; stage++)
	{
		pipeline->pipe_stages[stage].state = stage;
		pipeline->pipe_stages[stage].pc = UINT16_MAX;
	}

	pipeline->pipe_stages[FETCH].pc = 0;
}

void Pipeline_Execute(Pipeline_s* pipeline)
{
	execute_stages(pipeline);
	if (can_free_data_hazard_stall(pipeline))
	{
		free_data_hazard_stall(pipeline);
	}
}

void Pipeline_WriteToTrace(Pipeline_s* pipeline, FILE* trace_file)
{
	for (int stage = FETCH; stage < PIPELINE_SIZE; stage++)
	{
		if (pipeline->pipe_stages[stage].pc == UINT16_MAX)
			fprintf(trace_file, "--- ");
		else
			fprintf(trace_file, "%03X ", pipeline->pipe_stages[stage].pc);
	}
}

void Pipeline_BubbleCommands(Pipeline_s* pipeline)
{
	for (int stage = PIPELINE_SIZE - 1; stage > FETCH; stage--)
	{
		if (pipeline->pipe_stages[stage - 1].is_stalled) 
		{
			pipeline->pipe_stages[stage].pc = UINT16_MAX;
			break;
		}
		else
		{
			pipeline->pipe_stages[stage].pc = pipeline->pipe_stages[stage - 1].pc;
			pipeline->pipe_stages[stage].instruction.command =
				pipeline->pipe_stages[stage - 1].instruction.command;
			pipeline->pipe_stages[stage].operation = *pipeline->pipe_stages[stage - 1].operation;
			pipeline->pipe_stages[stage].execute_result = pipeline->pipe_stages[stage - 1].execute_result;
		}
	}
}

/************************************
* static implementation             *
************************************/
static void fetch(Pipeline_s* pipeline)
{
	pipeline->pipe_stages[FETCH].pc = *(pipeline->opcode_params.pc);
	pipeline->pipe_stages[FETCH].instruction.command = pipeline->insturcionts_p[*(pipeline->opcode_params.pc)];
	if (!pipe_stalled(pipeline))
	{
		*(pipeline->opcode_params.pc)+= 1;
	}
}

static void decode(Pipeline_s* pipeline)
{
	pipeline->pipe_stages[DECODE].operation =
		OpcodeMapping[pipeline->pipe_stages[DECODE].instruction.bits.opcode].OperationFunc;

	if (Opcode_IsBranchResulotion(pipeline->pipe_stages[DECODE].instruction.bits.opcode))
	{
		prepare_registers_params(pipeline, DECODE);
		pipeline->pipe_stages[DECODE].operation(&pipeline->opcode_params);
	}

	if (pipeline_needs_data_hazard_stall(pipeline))
	{
		init_data_hazard_stall(pipeline);
	}
}

static void execute(Pipeline_s* pipeline)
{
	uint8_t opcode = pipeline->pipe_stages[EXECUTE].instruction.bits.opcode;
	if (!Opcode_IsBranchResulotion(opcode) && !Opcode_IsMemoryCommand(opcode))
	{
		prepare_registers_params(pipeline, EXECUTE);
		pipeline->pipe_stages[EXECUTE].operation(&pipeline->opcode_params);
	}
}


static void mem(Pipeline_s* pipeline)
{
	uint8_t opcode = pipeline->pipe_stages[MEM].instruction.bits.opcode;
 	if (Opcode_IsMemoryCommand(opcode))
	{
		uint32_t address = pipeline->pipe_stages[MEM].instruction.bits.rs + 
			pipeline->pipe_stages[MEM].instruction.bits.rt;

		uint32_t* data = &pipeline->core_registers_p[pipeline->pipe_stages[MEM].instruction.bits.rd];
		bool response = opcode == LW ? Cache_ReadData(&pipeline->cache_data, address, data) :
			Cache_WriteData(&pipeline->cache_data, address, pipeline->pipe_stages[MEM].instruction.bits.rd);

		handle_mem_stall(pipeline, !response);
	}
}

static void writeback(Pipeline_s* pipeline)
{
	InstructionFormat_s instuction = { .command = pipeline->pipe_stages[WRITE_BACK].instruction.command };
	int index = instuction.bits.opcode == JAL ? PROGRAM_COUNTER_REGISTER_NUM : instuction.bits.rd;
	pipeline->core_registers_p[index] = pipeline->pipe_stages[WRITE_BACK].execute_result;
}

static void prepare_registers_params(Pipeline_s *pipeline, PipelineSM_e stage)
{
	InstructionFormat_s instuction = {.command = pipeline->pipe_stages[stage].instruction.command};
	pipeline->core_registers_p[IMMEDIATE_REGISTER_INDEX] = instuction.bits.immediate;
	pipeline->pipe_stages[stage].execute_result = pipeline->core_registers_p[instuction.bits.rd];

	pipeline->opcode_params.rs = pipeline->core_registers_p[instuction.bits.rs];
	pipeline->opcode_params.rt = pipeline->core_registers_p[instuction.bits.rt];
	pipeline->opcode_params.rd = &pipeline->pipe_stages[stage].execute_result;
}

static void execute_stages(Pipeline_s* pipeline)
{
	if (pipeline->memory_stall)
	{
		if(pipeline->pipe_stages[WRITE_BACK].pc != UINT16_MAX)
			pipe_functions[WRITE_BACK](pipeline);
		pipe_functions[MEM](pipeline);
	}

	else 
	{
		for (PipelineSM_e stage = 0; stage < PIPELINE_SIZE; stage++)
		{

			if (pipeline->pipe_stages[stage].is_stalled || pipeline->pipe_stages[stage].pc == UINT16_MAX)
			{
				continue;
			}

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

static bool check_registers_hazrads(Pipeline_s *pipeline, PipelineSM_e stage)
{
	if (pipeline->pipe_stages[stage].is_stalled)
	{
		return false;
	}
	return compare_register(pipeline, pipeline->pipe_stages[stage].instruction.bits.rd);
}

static bool pipeline_needs_data_hazard_stall(Pipeline_s* pipeline)
{
	return check_registers_hazrads(pipeline, EXECUTE) || check_registers_hazrads(pipeline, MEM) 
		|| check_registers_hazrads(pipeline, WRITE_BACK);
}

static void init_data_hazard_stall(Pipeline_s* pipeline)
{
	pipeline->data_hazard_stall = true;
	pipeline->pipe_stages[FETCH].is_stalled  = true;
	pipeline->pipe_stages[DECODE].is_stalled = true;
}

static void free_data_hazard_stall(Pipeline_s* pipeline)
{
	pipeline->data_hazard_stall = false;
	pipeline->pipe_stages[FETCH].is_stalled = false;
	pipeline->pipe_stages[DECODE].is_stalled = false;
}

bool can_free_data_hazard_stall(Pipeline_s *pipeline)
{
	return pipeline->data_hazard_stall && !pipeline_needs_data_hazard_stall(pipeline);
}

static void handle_mem_stall(Pipeline_s* pipeline, bool stall)
{
	pipeline -> memory_stall = stall;
	for (int stage = FETCH; stage < WRITE_BACK; stage++)
	{
		pipeline->pipe_stages[stage].is_stalled = stall;
	}
}

static bool pipe_stalled(Pipeline_s* pipeline)
{
	return pipeline->data_hazard_stall || pipeline->memory_stall;
}

