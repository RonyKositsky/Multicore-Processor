#define _CRT_SECURE_NO_WARNINGS
#include "Files.h"

#include <string.h>

/*
* Opening all of the required filed from the arguments.
*/
int OpenFiles(char* argv[])
{
	ImemInFile = fopen(argv[1], "r");
	DmemInFile = fopen(argv[2], "r");
	DiskInFile = fopen(argv[3], "r");
	Irq2InFile = fopen(argv[4], "r");
	DmemOutFile = fopen(argv[5], "w");
	RegOutFile = fopen(argv[6], "w");
	TraceFile = fopen(argv[7], "w");
	HwRegTraceFile = fopen(argv[8], "w");
	CyclesFile = fopen(argv[9], "w");
	LedsFile = fopen(argv[10], "w");
	MonitorFile = fopen(argv[11], "w");
	MonitorYuvFile = fopen(argv[12], "wb");
	DiskOutFile = fopen(argv[13], "w");

	return (ImemInFile == NULL) ||
		(DmemInFile == NULL) ||
		(ImemInFile == NULL) ||
		(DmemInFile == NULL) ||
		(DiskInFile == NULL) ||
		(Irq2InFile == NULL) ||
		(DmemOutFile == NULL) ||
		(RegOutFile == NULL) ||
		(TraceFile == NULL) ||
		(HwRegTraceFile == NULL) ||
		(CyclesFile == NULL) ||
		(LedsFile == NULL) ||
		(MonitorFile == NULL) ||
		(MonitorYuvFile == NULL) ||
		(DiskOutFile == NULL);
}

/*
* Closing all files at the end of the program.
*/
void CloseFiles()
{
	fclose(ImemInFile);
	fclose(DmemInFile);
	fclose(DiskInFile);
	fclose(Irq2InFile);
	fclose(DmemOutFile);
	fclose(RegOutFile);
	fclose(TraceFile);
	fclose(HwRegTraceFile);
	fclose(CyclesFile);
	fclose(LedsFile);
	fclose(MonitorFile);
	fclose(MonitorYuvFile);
	fclose(DiskOutFile);
}

/*
* Writing HW trace line. Retrive command as an arguments and exctract all the needed values from the command.
*/
void WriteHwTrace(InstructionCommand command)
{
	char* mode = strcmp(command.opcode.OpcodeName, "in") ? "WRITE" : "READ";
	uint registerIndex = RegisterMapping[command.rs].RegisterValue + RegisterMapping[command.rt].RegisterValue;

	uint value = !strcmp(command.opcode.OpcodeName, "in") ? 
		IORegisterMapping[registerIndex].RegisterValue :
		RegisterMapping[command.rd].RegisterValue;
		
	fprintf(HwRegTraceFile, "%d %s %s %08X\n", 
		ClockCycles +1, mode, IORegisterMapping[registerIndex].RegisterName, value);
}

/*
* Writing trace line. Retrive command as an arguments and exctract all the needed values from the command.
*/
void WriteTrace(InstructionCommand command)
{
	fprintf(TraceFile, "%03X %s", command.PCLocation, command.Name);

	for (int i = 0; i < NUMBER_OF_REGISTERS; i++)
	{
		fprintf(TraceFile, " %08X", RegisterMapping[i].RegisterValue);
	}
	fputc('\n', TraceFile);

	if (!strcmp(command.opcode.OpcodeName, "in") || !strcmp(command.opcode.OpcodeName, "out"))
		WriteHwTrace(command);
}

/*
* Writing all the regiters values at the end of the program.
*/
void WriteRegistersToFile()
{
	for (int i = 2; i < NUMBER_OF_REGISTERS; i++)
	{
		fprintf(RegOutFile, "%08X\n", RegisterMapping[i].RegisterValue);
	}
}

/*
* Writing clock cycles and total instruction conducted to cycles files.
*/
void WriteCyclesToFile()
{
	fprintf(CyclesFile, "%d\n", ClockCycles);
	fprintf(CyclesFile, "%d\n", TotalInstructionsCommand);
}
