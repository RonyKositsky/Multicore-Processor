#define _CRT_SECURE_NO_WARNINGS
/*!
******************************************************************************
\file Files.c
\date 17 October 2021
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
#include "../include/Files.h"
#include <stdbool.h>

/************************************
*      definitions                 *
************************************/

/************************************
*       types                       *
************************************/

/************************************
*      variables                    *
************************************/
Core_Files CoresFilesArray[NUMBER_OF_CORES];

/************************************
*      static functions             *
************************************/
static bool opened_core_files_successfully(Core_Files* core_files);
static void close_core_files(Core_Files* core_files);

/************************************
*       API implementation          *
************************************/
int FIles_TryToOpenFIles(char* argv[])
{
	memset(CoresFilesArray, 0, 4 * sizeof(Core_Files));
	bool relative_path_input = (sizeof(argv) / sizeof(argv[0])) == 1;
	
	MeminFile = relative_path_input ? fopen("memin.txt", "r") : fopen(argv[5], "r");
	MemoutFile = relative_path_input ? fopen("memout.txt", "w") : fopen(argv[6], "w");
	BusTraceFile = relative_path_input ? fopen("bustrace.txt", "w") : fopen(argv[15], "w");
	
	CoresFilesArray[0].InstructionMemFile = relative_path_input ? fopen("imem0.txt", "r") : fopen(argv[1], "r");
	CoresFilesArray[0].RegFile			  = relative_path_input ? fopen("regout0.txt", "w") : fopen(argv[7], "w");
	CoresFilesArray[0].TraceFile		  = relative_path_input ? fopen("core0trace.txt", "w") : fopen(argv[11], "w");
	CoresFilesArray[0].DsRamFile	  	  = relative_path_input ? fopen("dsram0.txt", "w") : fopen(argv[16], "w");
	CoresFilesArray[0].TsRamFile		  = relative_path_input ? fopen("tsram0.txt", "w") : fopen(argv[20], "w");
	CoresFilesArray[0].StatsFile		  = relative_path_input ? fopen("stats0.txt", "w") : fopen(argv[24], "w");

	CoresFilesArray[1].InstructionMemFile = relative_path_input ? fopen("imem1.txt", "r") : fopen(argv[2], "r");
	CoresFilesArray[1].RegFile			  = relative_path_input ? fopen("regout1.txt", "w") : fopen(argv[8], "w");
	CoresFilesArray[1].TraceFile		  = relative_path_input ? fopen("core1trace.txt", "w") : fopen(argv[12], "w");
	CoresFilesArray[1].DsRamFile		  = relative_path_input ? fopen("dsram1.txt", "w") : fopen(argv[17], "w");
	CoresFilesArray[1].TsRamFile		  = relative_path_input ? fopen("tsram1.txt", "w") : fopen(argv[21], "w");
	CoresFilesArray[1].StatsFile		  = relative_path_input ? fopen("stats1.txt", "w") : fopen(argv[25], "w");

	CoresFilesArray[2].InstructionMemFile = relative_path_input ? fopen("imem2.txt", "r") : fopen(argv[3], "r");
	CoresFilesArray[2].RegFile			  = relative_path_input ? fopen("regout2.txt", "w") : fopen(argv[9], "w");
	CoresFilesArray[2].TraceFile		  = relative_path_input ? fopen("core2trace.txt", "w") : fopen(argv[13], "w");
	CoresFilesArray[2].DsRamFile		  = relative_path_input ? fopen("dsram2.txt", "w") : fopen(argv[18], "w");
	CoresFilesArray[2].TsRamFile		  = relative_path_input ? fopen("tsram2.txt", "w") : fopen(argv[22], "w");
	CoresFilesArray[2].StatsFile	      = relative_path_input ? fopen("stats2.txt", "w") : fopen(argv[26], "w");

	CoresFilesArray[3].InstructionMemFile = relative_path_input ? fopen("imem3.txt", "r") : fopen(argv[4], "r");
	CoresFilesArray[3].RegFile			  = relative_path_input ? fopen("regout3.txt", "w") : fopen(argv[10], "w");
	CoresFilesArray[3].TraceFile		  = relative_path_input ? fopen("core3trace.txt", "w") : fopen(argv[14], "w");
	CoresFilesArray[3].DsRamFile		  = relative_path_input ? fopen("dsram3.txt", "w") : fopen(argv[19], "w");
	CoresFilesArray[3].TsRamFile		  = relative_path_input ? fopen("tsram3.txt", "w") : fopen(argv[23], "w");
	CoresFilesArray[3].StatsFile		  = relative_path_input ? fopen("stats3.txt", "w") : fopen(argv[27], "w");

	bool failed = false;
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		failed |= opened_core_files_successfully(&CoresFilesArray[i]);
	}

	return failed || MeminFile == NULL || MemoutFile == NULL || BusTraceFile == NULL;
}

void CloseFiles()
{
	fclose(MeminFile);
	fclose(MemoutFile);
	fclose(BusTraceFile);
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		close_core_files(&CoresFilesArray[i]);
	}
	
}

/************************************
* static implementation             *
************************************/

static bool opened_core_files_successfully(Core_Files* core_files)
{
	return core_files->DsRamFile == NULL || core_files->TsRamFile == NULL || core_files->InstructionMemFile == NULL ||
		core_files->RegFile == NULL || core_files->TraceFile == NULL || core_files->StatsFile == NULL;
}

static void close_core_files(Core_Files* core_files)
{
	fclose(core_files->DsRamFile);
	fclose(core_files->InstructionMemFile);
	fclose(core_files->RegFile);
	fclose(core_files->StatsFile);
	fclose(core_files->TraceFile);
	fclose(core_files->TsRamFile);
}




