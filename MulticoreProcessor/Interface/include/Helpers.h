#ifndef HELPER
#define HELPER

#include <stdint.h>
#define NUMBER_OF_CORES 4
#define NUMBER_OF_REGISTERS 16

typedef union
{
	struct
	{
		uint16_t immediate : 12;	// [0:11]  Immediate value
		uint16_t source_1 : 4;		// [12:15] src1 value
		uint16_t source_0 : 4;		// [16:19] src0 value
		uint16_t destination : 4;	// [20:23] src0 value
		uint16_t opcode : 8;		// [24:31] opcode value
	} bits;

	uint32_t command;
} InstructionFormat_s;

uint32_t ClockCycles; //Total clock cycles.
uint32_t TotalInstructionsCommand; //Total instructions counter.

int GetDecimalFromHex(char* hexValue);
int GetDecimalFromHex2Comp(char* hexValue);
void RemoveLastChar(char* str);


#endif __HELPER
