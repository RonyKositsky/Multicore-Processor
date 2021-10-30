/*!
******************************************************************************
\file
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
static void init_regiters(Core_s* core);
static void init_memory(Core_s* core);

/************************************
*       API implementation          *
************************************/
void InitCore(Core_s *core)
{
	memset((uint8_t *)core, 0, sizeof(core));

	core->ProgramCounter = 0;
	core->InstructionCounter = 0;
	init_regiters(core);
	init_memory(core);
}

int CoreHalted(Core_s *core)
{
	return core->ProgramCounter == 0;
}

/*
* Write final memory image to output file.
*/
void WriteMemoryToFile(Core_s *core)
{
	for (uint32_t i = 0; i < MEMORY_SIZE; i++)
		fprintf(core->Files.DsRamFile, "%08lx\n", core->Memory[i]);
}

void CoreIter(Core_s* core) 
{
	// fetch instruction

	// 
}


/************************************
* static implementation             *
************************************/
static void init_regiters(Core_s* core)
{
	// todo: memset();

	for (int i = 0; i < NUMBER_OF_REGISTERS; i++)
	{
		core->RegisterArray[i] = 0;
	}
}

static void init_memory(Core_s* core)
{
	uint16_t lineInProgram = 0;
	while (lineInProgram < MEMORY_SIZE && fscanf(core->Files.MemFile, "%08x", (uint32_t *)&(core->Memory[lineInProgram])) != EOF)
		lineInProgram++;
}