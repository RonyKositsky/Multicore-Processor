#ifndef FILES
#define FILES
#include <stdio.h>
#include "Registers.h"

FILE* ImemInFile;
FILE* DmemInFile; 
FILE* DiskInFile; 
FILE* Irq2InFile;
FILE* DmemOutFile; 
FILE* RegOutFile;
FILE* TraceFile;
FILE* HwRegTraceFile;
FILE* CyclesFile;
FILE* LedsFile;
FILE* MonitorFile;
FILE* MonitorYuvFile; 
FILE* DiskOutFile; 

int OpenFiles(char* argv[]);
void CloseFiles(void);

void WriteTrace(InstructionCommand command);
void WriteRegistersToFile(void);
void WriteCyclesToFile(void);

#endif
