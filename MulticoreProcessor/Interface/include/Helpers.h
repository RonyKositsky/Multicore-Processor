#ifndef HELPER
#define HELPER

#include <stdint.h>
#define NUMBER_OF_CORES 4
#define NUMBER_OF_REGISTERS 16

uint32_t ClockCycles; //Total clock cycles.
uint32_t TotalInstructionsCommand; //Total instructions counter.

int GetDecimalFromHex(char* hexValue);
int GetDecimalFromHex2Comp(char* hexValue);
void RemoveLastChar(char* str);


#endif __HELPER
