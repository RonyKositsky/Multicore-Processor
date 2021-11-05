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
//InstructionCommand* InstructionCommands;

/************************************
*      static functions             *
************************************/
static void init_memory(Core_s* core);
static void write_trace(Core_s* core);
static void write_regs_to_file(Core_s* core);

/************************************
*       API implementation          *
************************************/
void InitCore(Core_s *core)
{
	memset(core, 0, sizeof(Core_s));
	memset(&core->statistics, 0, sizeof(Statistics_s));
	memset(&core->register_array, 0, sizeof(NUMBER_OF_REGISTERS));
	init_memory(core);
	memset(&core->pipeline, 0, sizeof(Pipeline_s));
	core->pipeline.insturcionts = core->core_files.InstructionMemFile;
	Pipeline_Init(&core->pipeline);
	core->program_counter = 0;
}

int CoreHalted(Core_s *core)
{
	return core->program_counter == 0;
}

void CoreIter(Core_s* core) 
{
	Pipeline_Execute(&core->pipeline);
	write_trace(core);
}


/************************************
* static implementation             *
************************************/

static void init_memory(Core_s* core)
{
	memset(&core->core_files, 0, sizeof(INSTRUCTIONS_MEMORY_SIZE));
	uint16_t lineInProgram = 0; // Making sure we are not exceeding the memory image.
	while (lineInProgram < INSTRUCTIONS_MEMORY_SIZE && fscanf(core->core_files.InstructionMemFile, 
		"%08x", (uint32_t *)&(core->instructions_memory_image[lineInProgram])) != EOF)
		lineInProgram++;
}

static void write_trace(Core_s* core)
{
	fprintf(core->core_files.TraceFile, "%08X ", core->statistics.cycles);
	Pipeline_WriteToTrace(&core->pipeline, core->core_files.TraceFile);
	write_regs_to_file(core);
	fprintf(core->core_files.TraceFile, "\n");

}

static void write_regs_to_file(Core_s* core)
{
	for (int i = 2; i < NUMBER_OF_REGISTERS; i++) // We are not writing register 0 and 1.
	{
		fprintf(core->core_files.TraceFile, "%d ", core->register_array[i]);
	}
}

