#define _CRT_SECURE_NO_WARNINGS
/*!
******************************************************************************
\file MainMemory.c
\date 17 October 2021
\author Rony Kositsky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan
\par
ALL RIGHTS RESERVED
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "../include/MainMemory.h"
#include "../include/Files.h"
#include "../include/Bus.h"
//
#include <string.h>

/************************************
*      definitions                 *
************************************/

/************************************
*       types                       *
************************************/
typedef union
{
	uint32_t address;

	struct
	{
		uint16_t offset : 2;	// [0:1]
		uint32_t block : 18;	// [2:19]
	} fields;
} memory_addess_s;

/************************************
*      variables                    *
************************************/
static uint32_t gMemory[MAIN_MEMORY_SIZE];
static uint8_t counter;
static bool gMemoryTransaction;

/************************************
*      static functions             *
************************************/
static bool bus_transaction_handler(Bus_packet_s* packet, bool direct_transaction);

/************************************
*       API implementation          *
************************************/
void MainMemory_Init(void)
{
	memset((uint8_t*)gMemory, 0, sizeof(gMemory));
	counter = 0;
	gMemoryTransaction = false;

	uint16_t lineInProgram = 0;
	while (lineInProgram < MAIN_MEMORY_SIZE && fscanf(MeminFile, "%08x", (uint32_t*)&(gMemory[lineInProgram])) != EOF)
		lineInProgram++;

	Bus_RegisterMemoryCallback(bus_transaction_handler);
}

/************************************
* static implementation             *
************************************/
static bool bus_transaction_handler(Bus_packet_s* packet, bool direct_transaction)
{
	if (packet->bus_cmd == bus_no_command)
		return false;

	if (!gMemoryTransaction)
	{
		gMemoryTransaction = true;
		counter = !direct_transaction ? 0 : 16;
	}

	if (counter >= 16)
	{
		if (packet->bus_cmd == bus_busRd || packet->bus_cmd == bus_busRdX)
		{
			// send the memory value
			packet->bus_origid = bus_main_memory;
			packet->bus_cmd = bus_flush;
			packet->bus_data = gMemory[packet->bus_addr];
		}
		else if (packet->bus_cmd == bus_flush)
		{
			// write data to memory
			gMemory[packet->bus_addr] = packet->bus_data;
		}
		
		if (counter == 19)
			gMemoryTransaction = false;
		
		counter++;
		return true;
	}

	counter++;	
	return false;
}

