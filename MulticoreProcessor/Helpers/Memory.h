#ifndef MEMORY
#define MEMORY
#define MEMORY_SIZE 4096
#include "Helpers.h"
 
uint Memory[MEMORY_SIZE]; //Memoery image array.

void MemoryInit(void);
void WriteMemoryToFile(void);


#endif
