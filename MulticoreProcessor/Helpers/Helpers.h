#ifndef HELPER
#define HELPER

typedef unsigned int uint;

uint ProgramCounter; //The program counter.
uint ClockCycles; //Total clock cycles.
uint TotalInstructionsCommand; //Total instructions counter.

int GetDecimalFromHex(char* hexValue);
int GetDecimalFromHex2Comp(char* hexValue);
void RemoveLastChar(char* str);


#endif
