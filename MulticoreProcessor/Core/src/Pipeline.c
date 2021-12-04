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
*      static functions             *
************************************/
static void fetch(Pipeline_s* pipeline);
static void decode(Pipeline_s* pipeline);
static void execute(Pipeline_s* pipeline);
static void mem(Pipeline_s* pipeline);
static void writeback(Pipeline_s* pipeline);
static void execute_stages(Pipeline_s* pipeline);
static void prepare_registers_params(Pipeline_s* pipeline, PipelineSM_e stage);
static bool pipeline_needs_data_hazard_stall(Pipeline_s* pipeline);
static bool compare_register(Pipeline_s* pipeline, uint16_t reg, uint16_t stage);
static bool check_registers_hazrads(Pipeline_s* pipeline, PipelineSM_e stage);
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
	pipeline->data_hazard_stall = pipeline_needs_data_hazard_stall(pipeline);
	execute_stages(pipeline);
}

bool Pipeline_PipeFlushed(Pipeline_s* pipeline)
{
	bool flushed = pipeline->halted;
	for (int stage = FETCH; stage < PIPELINE_SIZE; stage++)
	{
		flushed &= (pipeline->pipe_stages[stage].pc == UINT16_MAX);
	}

	return flushed;
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
		if (pipeline->memory_stall)
		{
			pipeline->pipe_stages[WRITE_BACK].pc = UINT16_MAX;
			break;
		}
		else if (pipeline->data_hazard_stall && stage == EXECUTE)
		{
			pipeline->pipe_stages[EXECUTE].pc = UINT16_MAX;
			break;
		}
		else if (pipeline->pipe_stages[stage - 1].pc == UINT16_MAX)
		{
			pipeline->pipe_stages[stage].pc = UINT16_MAX;
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

	if (pipeline->halted)
	{
		pipeline->pipe_stages[FETCH].pc = UINT16_MAX;
		pipeline->pipe_stages[DECODE].pc = UINT16_MAX;
	}
}

/************************************
* static implementation             *
************************************/
static void fetch(Pipeline_s* pipeline)
{
	pipeline->pipe_stages[FETCH].pc = *(pipeline->opcode_params.pc);
	pipeline->pipe_stages[FETCH].instruction.command = pipeline->insturcionts_p[*(pipeline->opcode_params.pc)];
	if (!(pipeline->data_hazard_stall || pipeline->memory_stall)) // Not in stall
	{
		*(pipeline->opcode_params.pc)+= 1;
	}
}

static void decode(Pipeline_s* pipeline)
{
	uint8_t opcode = pipeline->pipe_stages[DECODE].instruction.bits.opcode;
	if (opcode == HALT)
	{
		pipeline->halted = true;
		return;
	}

	pipeline->pipe_stages[DECODE].operation = OpcodeMapping[opcode];

	if (Opcode_IsBranchResulotion(pipeline->pipe_stages[DECODE].instruction.bits.opcode))
	{
		prepare_registers_params(pipeline, DECODE);
		pipeline->pipe_stages[DECODE].operation(&pipeline->opcode_params);
	}
}

static void execute(Pipeline_s* pipeline)
{
	uint8_t opcode = pipeline->pipe_stages[EXECUTE].instruction.bits.opcode;
	if (!Opcode_IsBranchResulotion(opcode) && !Opcode_IsMemoryCommand(opcode) && opcode != HALT)
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
		prepare_registers_params(pipeline, MEM);
		uint32_t address = pipeline->opcode_params.rs + pipeline->opcode_params.rt;

		uint32_t* data = pipeline->opcode_params.rd;
		bool response = opcode == LW ? Cache_ReadData(&pipeline->cache_data, address, data) :
			Cache_WriteData(&pipeline->cache_data, address, *data);

		pipeline->memory_stall = !response;
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
	uint8_t stage = pipeline->memory_stall ? MEM : pipeline->data_hazard_stall ? EXECUTE : DECODE;
	if(!pipeline->halted)
		pipe_functions[FETCH](pipeline);

	for (; stage < PIPELINE_SIZE; stage++)
	{
		if (!(pipeline->pipe_stages[stage].pc == UINT16_MAX))
		{
			pipe_functions[stage](pipeline);
		}
	}
}

static bool compare_register(Pipeline_s* pipeline, uint16_t reg, uint16_t stage)
{
	bool ret = false;
	InstructionFormat_s decode_ins = pipeline->pipe_stages[DECODE].instruction;
	uint16_t op = pipeline->pipe_stages[WRITE_BACK].instruction.bits.opcode;

	if (reg == IMMEDIATE_REGISTER_INDEX || reg == ZERO_REGISTER_INDEX)
	{
		ret = false;
	}
	else if (decode_ins.bits.opcode <= SRL || decode_ins.bits.opcode == LW)
	{
		ret = (reg == decode_ins.bits.rs || reg == decode_ins.bits.rt);
	}
	else if (decode_ins.bits.opcode == SW && op == SW)
	{
		ret = (reg == decode_ins.bits.rs || reg == decode_ins.bits.rt);
	}
	else
	{
		ret = (reg == decode_ins.bits.rd || reg == decode_ins.bits.rs || reg == decode_ins.bits.rt);
	}

	return ret;
}

static bool check_registers_hazrads(Pipeline_s *pipeline, PipelineSM_e stage)
{
	if (pipeline->pipe_stages[stage].pc == UINT16_MAX)
	{
		return false;
	}
	return compare_register(pipeline, pipeline->pipe_stages[stage].instruction.bits.rd, stage);
}

static bool pipeline_needs_data_hazard_stall(Pipeline_s* pipeline)
{
	return check_registers_hazrads(pipeline, EXECUTE) || check_registers_hazrads(pipeline, MEM) 
		|| check_registers_hazrads(pipeline, WRITE_BACK);
}

