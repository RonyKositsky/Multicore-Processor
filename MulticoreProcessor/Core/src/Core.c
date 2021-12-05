/*!
******************************************************************************
\file Core.c
\date 26 October 2021
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
#define _CRT_SECURE_NO_WARNINGS
#include "../include/Core.h"
#include <stdio.h>
#include <string.h>

/************************************
*      definitions                 *
************************************/
#define INSTRUCTION_COUNT 0

/************************************
*      variables                    *
************************************/

/************************************
*      static functions             *
************************************/
static void init_memory(Core_s* core);
static void write_trace(Core_s* core, uint32_t* regs_copy);
static void write_regs_to_file(Core_s* core, uint32_t* regs_copy);
static void update_statistics(Core_s* core);
static void print_register_file(Core_s* core);
static void print_statistics(Core_s* core);

/************************************
*       API implementation          *
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
void Core_Init(Core_s *core, uint8_t id)
{
	core->program_counter = 0;
	core->index = id;
	core->core_halted = false;

	memset(&core->statistics, 0, sizeof(Statistics_s));
	core->statistics.cycles = -1; // To start the count from 0.

	memset(&core->register_array, 0, sizeof(NUMBER_OF_REGISTERS));
	init_memory(core);

	memset(&core->pipeline, 0, sizeof(Pipeline_s));
	Pipeline_Init(&core->pipeline);

	memset(&core->pipeline.cache_data, 0, sizeof(CacheData_s));
	Cache_Init(&core->pipeline.cache_data, id);

	//TODO: maybe move it to different location.
	Cache_RegisterBusHandles();

	core->pipeline.core_registers_p = core->register_array;
	core->pipeline.insturcionts_p = core->instructions_memory_image;
	core->pipeline.opcode_params.pc = &(core->program_counter);
}

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
void Core_Iter(Core_s* core)
{
	if (Pipeline_PipeFlushed(&core->pipeline))
	{
		core->core_halted = true;
		return;
	}

	uint32_t regs_copy[NUMBER_OF_REGISTERS];
	memcpy(regs_copy, core->register_array, sizeof(core->register_array));

	update_statistics(core);
	Pipeline_Execute(&core->pipeline);
	write_trace(core, regs_copy);
	Pipeline_BubbleCommands(&core->pipeline);
}

/*!
******************************************************************************
\brief
Teardown of the code.

\param
 [in] core - the operating core

\return none
*****************************************************************************/
void Core_Teaddown(Core_s* core)
{
	print_register_file(core);
	Cache_PrintData(&core->pipeline.cache_data,
		core->core_files.DsRamFile, core->core_files.TsRamFile);
	print_statistics(core);
}

/*!
******************************************************************************
\brief
If the core is halted.

\param
 [in] core - the operating core
 [out] bool

\return true if core is halted, fale otherwise.
*****************************************************************************/
bool Core_Halted(Core_s* core)
{
	return core->core_halted;
}


/************************************
* static implementation             *
************************************/

/*!
******************************************************************************
\brief
Init the core instructions memory.

\param
 [in] core - the operating core

\return none
*****************************************************************************/
static void init_memory(Core_s* core)
{
	uint16_t lineInProgram = 0; // Making sure we are not exceeding the memory image.
	while (lineInProgram < INSTRUCTIONS_MEMORY_SIZE && fscanf(core->core_files.InstructionMemFile, 
		"%08x", (uint32_t *)&(core->instructions_memory_image[lineInProgram])) != EOF)
		lineInProgram++;
}

/*!
******************************************************************************
\brief
Writing the trace of the core.

\param
 [in] core				  - the operating core
 [in] uint32_t *regs_copy - pointer to the regs copied values.

\return none
*****************************************************************************/
static void write_trace(Core_s* core, uint32_t *regs_copy)
{
	fprintf(core->core_files.TraceFile, "%d ", core->statistics.cycles);
	Pipeline_WriteToTrace(&core->pipeline, core->core_files.TraceFile);
	write_regs_to_file(core, regs_copy);
	fprintf(core->core_files.TraceFile, "\n");

}

/*!
******************************************************************************
\brief
Writing the registers to file.

\param
 [in] core				  - the operating core.
 [in] uint32_t *regs_copy - pointer to the regs copied values.

\return none
*****************************************************************************/
static void write_regs_to_file(Core_s* core, uint32_t* regs_copy)
{
	for (int i = START_MUTABLE_REGISTER_INDEX; i < NUMBER_OF_REGISTERS; i++) // We are not writing register 0 and 1.
	{
		fprintf(core->core_files.TraceFile, "%08X ", regs_copy[i]);
	}
}

/*!
******************************************************************************
\brief
Updating staistics struct.

\param
 [in] core - the operating core.

\return none
*****************************************************************************/
static void update_statistics(Core_s* core)
{
	core->statistics.cycles++;

	if (!core->pipeline.halted && !core->pipeline.memory_stall && !core->pipeline.data_hazard_stall)
		core->statistics.instructions++;
}

/*!
******************************************************************************
\brief
Printing the registers file.

\param
 [in] core - the operating core.

\return none
*****************************************************************************/
static void print_register_file(Core_s* core)
{
	for (int i = START_MUTABLE_REGISTER_INDEX; i < NUMBER_OF_REGISTERS; i++)
	{
		fprintf(core->core_files.RegFile, "%08X\n", core->register_array[i]);
	}
}

static void print_statistics(Core_s* core)
{
	fprintf(core->core_files.StatsFile, "cycles %d\n", core->statistics.cycles + 1);
	fprintf(core->core_files.StatsFile, "instructions %d\n", core->statistics.instructions - 1);
	fprintf(core->core_files.StatsFile, "read_hit %d\n", core->pipeline.cache_data.statistics.read_hits);
	fprintf(core->core_files.StatsFile, "write_hit %d\n", core->pipeline.cache_data.statistics.write_hits);
	fprintf(core->core_files.StatsFile, "read_miss %d\n", core->pipeline.cache_data.statistics.read_misses);
	fprintf(core->core_files.StatsFile, "write_miss %d\n", core->pipeline.cache_data.statistics.write_misses);
	fprintf(core->core_files.StatsFile, "decode_stall %d\n", core->pipeline.statistics.decode_stalls);
	fprintf(core->core_files.StatsFile, "mem_stall %d\n", core->pipeline.statistics.mem_stalls);
}


