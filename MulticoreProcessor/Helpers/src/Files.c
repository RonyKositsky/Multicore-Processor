#define _CRT_SECURE_NO_WARNINGS

#include "../include/Files.h"

/*
* Opening all of the required filed from the arguments.
*/
int OpenFiles(char* argv[])
{
	Imem0File = fopen(argv[1], "r");
	Imem1File = fopen(argv[2], "r");
	Imem2File = fopen(argv[3], "r");
	Imem3File = fopen(argv[4], "r");
	MeminFile = fopen(argv[5], "r");
	MemoutFile = fopen(argv[6], "w");
	Regout0File = fopen(argv[7], "w");
	Regout1File = fopen(argv[8], "w");
	Regout2File = fopen(argv[9], "w");
	Regout3File = fopen(argv[10], "w");
	Core0TraceFile = fopen(argv[11], "w");
	Core1TraceFile = fopen(argv[12], "w");
	Core2TraceFile = fopen(argv[13], "w");
	Core3TraceFile = fopen(argv[14], "w");
	BusTraceFile = fopen(argv[15], "w");
	DsRam0TraceFile = fopen(argv[16], "w");
	DsRam1TraceFile = fopen(argv[17], "w");
	DsRam2TraceFile = fopen(argv[18], "w");
	DsRam3TraceFile = fopen(argv[19], "w");
	TsRam0TraceFile = fopen(argv[20], "w");
	TsRam1TraceFile = fopen(argv[21], "w");
	TsRam2TraceFile = fopen(argv[22], "w");
	TsRam3TraceFile = fopen(argv[23], "w");
	Stats0TraceFile = fopen(argv[24], "w");
	Stats1TraceFile = fopen(argv[25], "w");
	Stats2TraceFile = fopen(argv[26], "w");
	Stats3TraceFile = fopen(argv[27], "w");

	return Imem0File == NULL || Imem1File == NULL || Imem2File == NULL || Imem3File == NULL ||
		   MeminFile == NULL || MemoutFile == NULL || Regout0File == NULL || Regout1File == NULL ||
		   Regout2File == NULL || Regout3File == NULL || Core0TraceFile == NULL || Core1TraceFile == NULL ||
		   Core2TraceFile == NULL || Core3TraceFile == NULL || BusTraceFile == NULL || DsRam0TraceFile == NULL ||
		   DsRam1TraceFile == NULL || DsRam2TraceFile == NULL || DsRam3TraceFile == NULL || TsRam0TraceFile == NULL ||
		   TsRam1TraceFile == NULL || TsRam2TraceFile == NULL || TsRam3TraceFile == NULL || Stats0TraceFile == NULL ||
		   Stats1TraceFile == NULL || Stats2TraceFile == NULL || Stats3TraceFile == NULL;
}

/*
* Closing all files at the end of the program.
*/
void CloseFiles()
{
	fclose(Imem0File);
	fclose(Imem1File);
	fclose(Imem2File);
	fclose(Imem3File);
	fclose(MeminFile);
	fclose(MemoutFile);
	fclose(Regout0File);
	fclose(Regout1File);
	fclose(Regout2File);
	fclose(Regout3File);
	fclose(Core0TraceFile);
	fclose(Core1TraceFile);
	fclose(Core2TraceFile);
	fclose(Core3TraceFile);
	fclose(BusTraceFile);
	fclose(DsRam0TraceFile);
	fclose(DsRam1TraceFile);
	fclose(DsRam2TraceFile);
	fclose(DsRam3TraceFile);
	fclose(TsRam0TraceFile);
	fclose(TsRam1TraceFile);
	fclose(TsRam2TraceFile);
	fclose(TsRam3TraceFile);
	fclose(Stats0TraceFile);
	fclose(Stats1TraceFile);
	fclose(Stats2TraceFile);
	fclose(Stats3TraceFile);
}

void AssignFiles(Core_s *cores)
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

/*
* Writing HW trace line. Retrive command as an arguments and exctract all the needed values from the command.
*/
void WriteHwTrace()
{
	/*char* mode = strcmp(command.opcode.OpcodeName, "in") ? "WRITE" : "READ";
	uint32_t registerIndex = RegisterMapping[command.rs].RegisterValue + RegisterMapping[command.rt].RegisterValue;

	uint32_t value = !strcmp(command.opcode.OpcodeName, "in") ? 
		IORegisterMapping[registerIndex].RegisterValue :
		RegisterMapping[command.rd].RegisterValue;
		
	fprintf(HwRegTraceFile, "%d %s %s %08X\n", 
		ClockCycles +1, mode, IORegisterMapping[registerIndex].RegisterName, value);*/
}

/*
* Writing trace line. Retrive command as an arguments and exctract all the needed values from the command.
*/
void WriteTrace()
{
	
}

/*
* Writing all the regiters values at the end of the program.
*/
void WriteRegistersToFile()
{

}

/*
* Writing clock cycles and total instruction conducted to cycles files.
*/
void WriteCyclesToFile()
{
	
}
