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
#include "Core.h"
#include <cstddef>

/************************************
*      definitions                 *
************************************/
#define INSTRUCTION_COUNT 0

/************************************
*      variables                    *
************************************/
Core data_s;
InstructionCommand* InstructionCommands;

void InitRegiters(Core* core)
{
	for (int i = 0; i < NUMBER_OF_REGISTERS; i++)
	{
		core->RegisterArray[i] = 0;
	}
}

/************************************
*       API implementation          *
************************************/
void InitCore(Core *core)
{
	core->ProgramCounter = 0;
	core->InstructionCounter = 0;
	InitRegiters(core);
}

int CoreHalted(Core *core)
{
	return core->ProgramCounter == 0;
}

/*
* Init memory array from the input file.
*/
void MemoryInit()
{
	char line[10];
	int lineCounter = 0;
	//while (fgets(line, 10, DmemInFile) != NULL) 
	//{
	//	data_s.Memory[lineCounter] = strtoul(line, NULL, 16);
	//	lineCounter++;
	//}

	while (lineCounter < MEMORY_SIZE) 
	{
		data_s.Memory[lineCounter] = 0;
		lineCounter++;
	}
}

/*
* Write final memory image to output file.
*/
void WriteMemoryToFile()
{
	for (uint i = 0; i < MEMORY_SIZE; i++)
	{
		//fprintf(DmemOutFile, "%08X\n", data_s.Memory[i]);
	}
}

/*

void InstructionInit()
{
	char line[7];
	int lineCounter = 0;
	while (fgets(line, 7, ImemInFile) != NULL) {
		char opcodeBytes[3] = { line[0], line[1], '\0' };
		uint opcodeIndex = GetDecimalFromHex(opcodeBytes);
		char rdBytes[2] = { line[2], '\0' };
		uint rdIndex = GetDecimalFromHex(rdBytes);
		char rsBytes[2] = { line[3], '\0' };
		uint rsIndex = GetDecimalFromHex(rsBytes);
		char rtBytes[2] = { line[4], '\0' };
		uint rtIndex = GetDecimalFromHex(rtBytes);
		uint pcLocation = lineCounter;
		lineCounter++;

		char* name = (char*)calloc(6, sizeof(char));
		if (name == NULL)
			exit(1);

		RemoveLastChar(line);
		strcpy(name, line);

		uint immValue = 0, hasImm = 0;
		if (rdIndex == 1 || rtIndex == 1 || rsIndex == 1)
		{
			if (fgets(line, 7, ImemInFile) != NULL)
			{
				immValue = GetDecimalFromHex2Comp(line);
			}
			lineCounter++;
			hasImm = 1;
		}

		InstructionCommand instruction = {
			pcLocation,
			OpcodeMapping[opcodeIndex],
			rdIndex,
			rsIndex,
			rtIndex,
			immValue,
			hasImm,
			name
		};

		if (InstructionCommands == NULL)
		{
			InstructionCommands = (InstructionCommand*)calloc(1, sizeof(InstructionCommand));
			if (InstructionCommands != NULL)
				*InstructionCommands = instruction;
			else
			{
				printf("Allocation failed");
				exit(1);
			}
		}
		else
		{
			InstructionCommands = realloc(InstructionCommands, (InstructionCounter + 1) * sizeof(InstructionCommand));
			if (InstructionCommands + InstructionCounter != NULL)
				*(InstructionCommands + InstructionCounter) = instruction;
			else
			{
				printf("Allocation failed");
				exit(1);
			}
		}
		InstructionCounter++;
	}
}


InstructionCommand* GetInstructionCommand(uint pc)
{
	for (uint i = 0; i < InstructionCounter; i++)
	{
		if (InstructionCommands[i].PCLocation == pc)
			return &InstructionCommands[i];
	}

	return NULL;
}

void FreeInstructionsCommandArray()
{
	for (uint i = 0; i < InstructionCounter; i++)
	{
		free((InstructionCommands + i)->Name);
	}

	if (InstructionCommands != NULL)
		free(InstructionCommands);
}

*/
