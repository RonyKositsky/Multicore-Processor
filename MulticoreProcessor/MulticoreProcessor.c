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
static void CoresInit();



/************************************
*      Main							*
************************************/
int main(int argc, char *argv[])
{
	if (FIles_TryToOpenFIles(argv, argc))
	{
		printf("Error in opening files.\n");
		return 1;
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

/*!
******************************************************************************
\brief
Returns wether the processor halted.

\param
 [in] none
 [out] bool

\return true if thie is branch resulotion, false otherwise.
*****************************************************************************/
static bool ProcessorHalted()
{
	bool is_halted = true;
	for (int core = 0; core < NUMBER_OF_CORES; core++)
	{
		is_halted &= Core_Halted(&cores[core]);
	}

	return is_halted;
}

/*!
******************************************************************************
\brief
Returns wether the processor halted.

\param
 [in] Core_s* cores - pointer to the cores array.
 [out] none

 *****************************************************************************/
static void AssignFiles(Core_s* cores)
{
	for (int core = 0; core < NUMBER_OF_CORES; core++)
	{
		cores[core].core_files = CoresFilesArray[core];
	}
}

/*!
******************************************************************************
\brief
Init all the cores.

\param
 [in] none
 [out] none

*****************************************************************************/
void CoresInit()
{
	memset(cores, 0, NUMBER_OF_CORES * sizeof(Core_s));
	AssignFiles(cores);
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		Core_Init(&cores[i], i);
	}
}
