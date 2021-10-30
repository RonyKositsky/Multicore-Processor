#ifndef HELPER
#define HELPER

#include <stdint.h>

uint32_t ClockCycles; //Total clock cycles.
uint32_t TotalInstructionsCommand; //Total instructions counter.

int GetDecimalFromHex(char* hexValue);
int GetDecimalFromHex2Comp(char* hexValue);
void RemoveLastChar(char* str);


#endif __HELPER
