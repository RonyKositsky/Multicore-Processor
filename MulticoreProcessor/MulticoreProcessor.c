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
static bool IsHalted();
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

	int i = 0;
	//while (!IsHalted())
	while(i < 20)
	{
		for (int i = 0; i < NUMBER_OF_CORES; i++)
		{
			Core_Iter(&cores[i]);
		}
		i++;
	}

	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		Core_Teaddown(&cores[i]);
	}

	CloseFiles();
	return 0;
} 

/************************************
* static implementation             *
************************************/
static bool IsHalted()
{
	bool is_halted = true;
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		is_halted &= cores[i].pipeline.halted;
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
