#define _CRT_SECURE_NO_WARNINGS

#include "Registers.h"
#include "Files.h"
#include "Memory.h"

#include <stdlib.h>
#include <string.h>


/*
* Static defintion of all of our registers.
*/
Register RegisterMapping[NUMBER_OF_REGISTERS] = {
	{"$zero", "0", 0},		// Constant zero
	{"$imm", "1", 0},		// Sign extended immediate
	{"$v0", "2", 0},		// Result value
	{"$a0", "3", 0},		// Argument register
	{"$a1", "4", 0},		// Argument register
	{"$t0", "5", 0},		// Temporary register
	{"$t1", "6", 0},		// Temporary register
	{"$t2", "7", 0},		// Temporary register
	{"$t3", "8", 0},		// Temporary register
	{"$s0", "9", 0},		// Saved register
	{"$s1", "A", 0},		// Saved register
	{"$s2", "B", 0},		// Saved register
	{"$gp", "C", 0},		// Global pointer (static data)
	{"$sp", "D", 0},		// Stack pointer
	{"$fp", "E", 0},		// Frame Pointer
	{"$ra", "F", 0},		// Return address
};

/*
* Functions for all of our opcodes operations.
*/
void Add(uint rd, uint rs, uint rt){
	RegisterMapping[rd].RegisterValue = 
		RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue; }
void Sub(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue - RegisterMapping[rt].RegisterValue; }
void And(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue & RegisterMapping[rt].RegisterValue; }
void Or(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue | RegisterMapping[rt].RegisterValue; }
void Xor(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue ^ RegisterMapping[rt].RegisterValue; }
void Multiply(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue * RegisterMapping[rt].RegisterValue; }
void LogicShiftLeft(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue << RegisterMapping[rt].RegisterValue; }
void ArithmeticShiftRight(uint rd, uint rs, uint rt)
{
	RegisterMapping[rd].RegisterValue =
		(int)RegisterMapping[rs].RegisterValue >> (int)RegisterMapping[rt].RegisterValue;
} 
void LogicShiftRight(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue >> RegisterMapping[rt].RegisterValue; }
void BranchEqual(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue == RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchNotEqual(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue != RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchLessThen(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue < RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchGraterThen(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue > RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchLessEqual(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue <= RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchGraterEqual(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue >= RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void JumpAndLink(uint rd, uint rs, uint rt) {
	RegisterMapping[15].RegisterValue = ProgramCounter;
	ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF;
}
void LoadWord(uint rd, uint rs, uint rt) {
	uint address = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	RegisterMapping[rd].RegisterValue = Memory[address];
}
void StoreWord(uint rd, uint rs, uint rt) {
	uint address = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	Memory[address] = RegisterMapping[rd].RegisterValue;
}
void ReturnInterrupt(uint rd, uint rs, uint rt) {
	ProgramCounter = IORegisterMapping[IRQ_RETURN].RegisterValue;
}
void In(uint rd, uint rs, uint rt) {
	uint ioIndex = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	RegisterMapping[rd].RegisterValue = IORegisterMapping[ioIndex].RegisterValue;
}
void Out(uint rd, uint rs, uint rt) {
	uint ioIndex = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	IORegisterMapping[ioIndex].RegisterValue = RegisterMapping[rd].RegisterValue;
}
void Halt(uint rd, uint rs, uint rt)
{
	ProgramCounter = INSTRUCTION_COUNT + 1;
}

/*
* Opcode mapping array.
*/
Opcode OpcodeMapping[NUMBER_OF_OPCODES] = {
	{"add", "00", Add},
	{"sub", "01", Sub},
	{"and", "02", And},
	{"or", "03", Or},
	{"xor", "04", Xor},
	{"mul", "05", Multiply},
	{"sll", "06", LogicShiftLeft},
	{"sra", "07", ArithmeticShiftRight},
	{"srl", "08", LogicShiftRight},
	{"beq", "09", BranchEqual},
	{"bne", "0A", BranchNotEqual},
	{"blt", "0B", BranchLessThen},
	{"bgt", "0C", BranchGraterThen},
	{"ble", "0D", BranchLessEqual},
	{"bge", "0E", BranchGraterEqual},
	{"jal", "0F", JumpAndLink},
	{"lw", "10", LoadWord},
	{"sw", "11", StoreWord},
	{"reti", "12", ReturnInterrupt},
	{"in", "13", In},
	{"out", "14", Out},
	{"halt", "15", Halt},
};

/*
* Init all of our commands from the assembler to InstructionCommands array.
* During the main loop we will extract the next command from this array by its PC and will execute
* its command.
*/
void InstructionInit()
{
	char line[7];
	int lineCounter = 0;
	while (fgets(line, 7, ImemInFile) != NULL) {
		char opcodeBytes[3] = { line[0], line[1], '\0' };
		uint opcodeIndex = GetDecimalFromHex(opcodeBytes);
		char rdBytes[2] = { line[2], '\0' };
		uint rdIndex = GetDecimalFromHex(rdBytes);
		char rsBytes[2] = { line[3], '\0' };
		uint rsIndex = GetDecimalFromHex(rsBytes);
		char rtBytes[2] = { line[4], '\0' };
		uint rtIndex = GetDecimalFromHex(rtBytes);
		uint pcLocation = lineCounter;
		lineCounter++;

		char* name = (char*)calloc(6, sizeof(char));
		if (name == NULL)
			exit(1);

		RemoveLastChar(line);
		strcpy(name, line);

		uint immValue = 0, hasImm = 0;
		if (rdIndex == 1 || rtIndex == 1 || rsIndex == 1)
		{
			if (fgets(line, 7, ImemInFile) != NULL)
			{
				immValue = GetDecimalFromHex2Comp(line);
			}
			lineCounter++;
			hasImm = 1;
		}

		InstructionCommand instruction = {
			pcLocation,
			OpcodeMapping[opcodeIndex],
			rdIndex,
			rsIndex,
			rtIndex,
			immValue,
			hasImm,
			name
		};

		if (InstructionCommands == NULL)
		{
			InstructionCommands = (InstructionCommand*)calloc(1, sizeof(InstructionCommand));
			if (InstructionCommands != NULL)
				*InstructionCommands = instruction;
			else
			{
				printf("Allocation failed");
				exit(1);
			} 
		}
		else
		{
			InstructionCommands = realloc(InstructionCommands, (InstructionCounter + 1) * sizeof(InstructionCommand));
			if (InstructionCommands + InstructionCounter != NULL)
				*(InstructionCommands + InstructionCounter) = instruction;
			else
			{
				printf("Allocation failed");
				exit(1);
			}
		}
		InstructionCounter++;
	}
}

/*
* Retrive the command from our command array by its program counter.
*/
InstructionCommand* GetInstructionCommand(uint pc)
{
	for (uint i = 0; i < InstructionCounter; i++)
	{
		if (InstructionCommands[i].PCLocation == pc)
			return &InstructionCommands[i];
	}
	
	return NULL;
}

/*
* Increase by 2 or 1 depends if the command has immediate value.
*/
uint IncreasePCAmount(InstructionCommand command)
{
	return command.HasImmediate ? 2 : 1;
}

/*
* Free all dynamic memory allocation of the InstructionCommands array.
*/
void FreeInstructionsCommandArray()
{
	for (uint i = 0; i < InstructionCounter; i++)
	{
		free((InstructionCommands + i)->Name);
	}

	if (InstructionCommands != NULL)
		free(InstructionCommands);
}
