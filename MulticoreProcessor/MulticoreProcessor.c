/*!
******************************************************************************
\file MulticoreProcessor.c
\date 26 October 2021
\author Rony Kositsky & Ofir Guthman & Yonatan Gartenberg
\brief

The main file for our program.

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan
\par
ALL RIGHTS RESERVED
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "Interface/include/Files.h"
#include "Core/include/Core.h"
#include "Interface/include/MainMemory.h"
#include "Interface/include/Bus.h"
#include <string.h>

/************************************
*      definitions                  *
************************************/

/************************************
*      variables                  *
************************************/
static Core_s cores[NUMBER_OF_CORES];

/************************************
*      static functions             *
************************************/
static bool ProcessorHalted();
static void AssignFiles(Core_s* cores);

/************************************
*      functions					*
************************************/
void CoresInit()
{
	memset(cores, 0, NUMBER_OF_CORES * sizeof(Core_s));
	AssignFiles(cores);
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		Core_Init(&cores[i], i);
	}
}

/************************************
*      Main							*
************************************/
int main(int argc, char *argv[])
{
	if (FIles_TryToOpenFIles(argv, argc))
	{
		//Handle files error.
	}
	MainMemory_Init();
	CoresInit();

	while (!ProcessorHalted())
	{
		Bus_Iter();
		for (int core = 0; core < NUMBER_OF_CORES; core++)
		{
			Core_Iter(&cores[core]);
		}
	}

	for (int core = 0; core < NUMBER_OF_CORES; core++)
	{
		Core_Teaddown(&cores[core]);
	}

	CloseFiles();
	return 0;
} 

/************************************
* static implementation             *
************************************/
static bool ProcessorHalted()
{
	bool is_halted = true;
	for (int core = 0; core < NUMBER_OF_CORES; core++)
	{
		is_halted &= Core_Halted(&cores[core]);
	}

	return is_halted;
}

static void AssignFiles(Core_s* cores)
{
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		cores[i].core_files = CoresFilesArray[i];
	}
}
