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
	// first assign all files into the cores.
	AssignFiles(cores);

	// init all cores regs and memory.
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		InitCore(&cores[i]);
	}

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
	cores[0].Files.MemFile = Imem0File;
	cores[0].Files.RegFile = Regout0File;
	cores[0].Files.TraceFile = Core0TraceFile;
	cores[0].Files.DsRamFile = DsRam0TraceFile;
	cores[0].Files.TsRamFile = TsRam0TraceFile;
	cores[0].Files.StatsFile = Stats0TraceFile;

	//Core_s 2
	cores[1].Files.MemFile = Imem1File;
	cores[1].Files.RegFile = Regout1File;
	cores[1].Files.TraceFile = Core1TraceFile;
	cores[1].Files.DsRamFile = DsRam1TraceFile;
	cores[1].Files.TsRamFile = TsRam1TraceFile;
	cores[1].Files.StatsFile = Stats1TraceFile;

	//Core_s 3
	cores[2].Files.MemFile = Imem2File;
	cores[2].Files.RegFile = Regout2File;
	cores[2].Files.TraceFile = Core2TraceFile;
	cores[2].Files.DsRamFile = DsRam2TraceFile;
	cores[2].Files.TsRamFile = TsRam2TraceFile;
	cores[2].Files.StatsFile = Stats2TraceFile;

	//Core_s 4
	cores[3].Files.MemFile = Imem3File;
	cores[3].Files.RegFile = Regout3File;
	cores[3].Files.TraceFile = Core3TraceFile;
	cores[3].Files.DsRamFile = DsRam3TraceFile;
	cores[3].Files.TsRamFile = TsRam3TraceFile;
	cores[3].Files.StatsFile = Stats3TraceFile;
}