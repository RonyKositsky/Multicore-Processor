#include "Memory.h"
#include <stdlib.h>
#include "Files.h"

/*
* Init memory array from the input file.
*/
void MemoryInit()
{
	char line[10];
	int lineCounter = 0;
	while (fgets(line, 10, DmemInFile) != NULL) {
		//Memory[lineCounter] = GetDecimalFromHex(line);
		Memory[lineCounter] = strtoul(line, NULL, 16);
		lineCounter++;
	}

	while (lineCounter < MEMORY_SIZE) {
		Memory[lineCounter] = 0;
		lineCounter++;
	}
}

/*
* Write final memory image to output file.
*/
void WriteMemoryToFile()
{
	for (uint i = 0; i < MEMORY_SIZE; i++)
	{
		fprintf(DmemOutFile, "%08X\n", Memory[i]);
	}
}


