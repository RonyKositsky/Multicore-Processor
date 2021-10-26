#ifndef REGISTERS
#define REGISTERS

#define NUMBER_OF_REGISTERS 16
#define NUMBER_OF_OPCODES 22
#define INSTRUCTION_COUNT (InstructionCommands + InstructionCounter - 1)->PCLocation
#include "Helpers.h"

typedef struct
{
	char* RegisterName;
	char* RegisterNumber;
	uint RegisterValue;
}Register;

typedef struct
{
	char* OpcodeName;
	char* OpcodeNumber;
	void (*OperationFunc)(uint rd, uint rs, uint rt);
}Opcode;

typedef struct
{
	uint PCLocation;
	Opcode opcode;
	uint rd;
	uint rs;
	uint rt;
	int ImmValue;
	int HasImmediate;
	char* Name;
}InstructionCommand;

InstructionCommand* InstructionCommands;
uint InstructionCounter;

Register RegisterMapping[NUMBER_OF_REGISTERS];
Opcode OpcodeMapping[NUMBER_OF_OPCODES];

void InstructionInit(void);
InstructionCommand* GetInstructionCommand(uint pc);
uint IncreasePCAmount(InstructionCommand command);

void FreeInstructionsCommandArray(void);

#endif
