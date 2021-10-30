#ifndef FILES_H_
#define FILES_H_

#include <stdio.h>
#include "Core.h"

FILE* Imem0File;
FILE* Imem1File;
FILE* Imem2File;
FILE* Imem3File;
FILE* MeminFile;
FILE* MemoutFile;
FILE* Regout0File;
FILE* Regout1File;
FILE* Regout2File;
FILE* Regout3File;
FILE* Core0TraceFile;
FILE* Core1TraceFile;
FILE* Core2TraceFile;
FILE* Core3TraceFile;
FILE* BusTraceFile;
FILE* DsRam0TraceFile;
FILE* DsRam1TraceFile;
FILE* DsRam2TraceFile;
FILE* DsRam3TraceFile;
FILE* TsRam0TraceFile;
FILE* TsRam1TraceFile;
FILE* TsRam2TraceFile;
FILE* TsRam3TraceFile;
FILE* Stats0TraceFile;
FILE* Stats1TraceFile;
FILE* Stats2TraceFile;
FILE* Stats3TraceFile;

int OpenFiles(char* argv[]);
void CloseFiles(void);

void WriteTrace();
void WriteRegistersToFile(void);
void WriteCyclesToFile(void);
void AssignFiles(Core_s* cores);

#endif __FILES_H_
