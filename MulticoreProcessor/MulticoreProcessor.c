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
static void AssignFiles(Core_s* cores);
static bool IsHalted();

/************************************
*      functions					*
************************************/
void CoresInit()
{
	memset(cores, 0, NUMBER_OF_CORES * sizeof(Core_s));
	AssignFiles(cores);
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		Core_Init(&cores[i]);
	}
}

/************************************
*      Main							*
************************************/
int main(int argc, char *argv[])
{
	OpenFiles(argv);
	CoresInit();
	int i = 0;
	//while (!IsHalted())
	while(i < 4)
	{
		//for (int i = 0; i < NUMBER_OF_CORES; i++)
		for (int i = 0; i < 1; i++)
		{
			Core_Iter(&cores[i]);
		}
		i++;
	}
	CloseFiles();
	return 0;
} 


/************************************
* static implementation             *
************************************/
static void AssignFiles(Core_s* cores)
{
	//Core_s 1
	cores[0].core_files.InstructionMemFile = Imem0File;
	cores[0].core_files.RegFile = Regout0File;
	cores[0].core_files.TraceFile = Core0TraceFile;
	cores[0].core_files.DsRamFile = DsRam0TraceFile;
	cores[0].core_files.TsRamFile = TsRam0TraceFile;
	cores[0].core_files.StatsFile = Stats0TraceFile;

	//Core_s 2
	cores[1].core_files.InstructionMemFile = Imem1File;
	cores[1].core_files.RegFile = Regout1File;
	cores[1].core_files.TraceFile = Core1TraceFile;
	cores[1].core_files.DsRamFile = DsRam1TraceFile;
	cores[1].core_files.TsRamFile = TsRam1TraceFile;
	cores[1].core_files.StatsFile = Stats1TraceFile;

	//Core_s 3
	cores[2].core_files.InstructionMemFile = Imem2File;
	cores[2].core_files.RegFile = Regout2File;
	cores[2].core_files.TraceFile = Core2TraceFile;
	cores[2].core_files.DsRamFile = DsRam2TraceFile;
	cores[2].core_files.TsRamFile = TsRam2TraceFile;
	cores[2].core_files.StatsFile = Stats2TraceFile;

	//Core_s 4
	cores[3].core_files.InstructionMemFile = Imem3File;
	cores[3].core_files.RegFile = Regout3File;
	cores[3].core_files.TraceFile = Core3TraceFile;
	cores[3].core_files.DsRamFile = DsRam3TraceFile;
	cores[3].core_files.TsRamFile = TsRam3TraceFile;
	cores[3].core_files.StatsFile = Stats3TraceFile;
}

static bool IsHalted()
{
	bool is_halted = true;
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		is_halted &= cores[i].pipeline.halted;
	}

	return is_halted;
}
