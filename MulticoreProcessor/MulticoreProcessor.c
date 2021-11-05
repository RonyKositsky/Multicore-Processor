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


/************************************
*      functions					*
************************************/
void CoresInit()
{
	// init all cores regs and memory.
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		InitCore(&cores[i]);
	}

	// first assign all files into the cores.
	AssignFiles(cores);

}

/************************************
*      Main							*
************************************/
int main(int argc, char *argv[])
{
	OpenFiles(argv);

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