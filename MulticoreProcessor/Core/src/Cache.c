/*!
******************************************************************************
\file Cache.c
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
#include "..\include\Cache.h"
#include <string.h>
#include "..\..\Interface\include\Bus.h"

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
		uint16_t index : 6;	// [2:7]
		uint16_t tag : 12;	// [8:19]
	} fields;
} CacheAddess_s;

/************************************
*      variables                    *
************************************/

/************************************
*      static functions             *
************************************/
static uint16_t get_block_number(CacheAddess_s addr);

/************************************
*       API implementation          *
************************************/
void Cache_Init(CacheData_s* data)
{
	// set all cache memory to 0;
	memset((uint8_t*)data, 0, sizeof(data));
}

bool Cache_ReadData(CacheData_s* cache_data, uint32_t address, uint32_t* data)
{
	CacheAddess_s addr;
	addr.address = address;

	// check if addresss tag is locate on block_number
	if (cache_data->tsram[addr.fields.index].fields.tag == addr.fields.tag) 
	{
		// hit on cache, getting the value 
		uint16_t index = addr.fields.index * 4 + addr.fields.offset;
		*data = cache_data->dsram[index];

		return true;
	}
	// we had a miss.
	
	// first, check if the required block is dirty
	// if so, we need first to send the block into the memory
	if (cache_data->tsram[addr.fields.index].fields.mesi == cache_mesi_modified)
	{

	}

	// now, we need to take the new block from the main memory.
	Bus_wires_s wire;
	wire.bus_origid = cache_data->id;
	wire.bus_addr = addr.address;
	wire.bus_cmd = bus_busRd;		// TODO: check if this right

	Bus_AddTransaction(wire);

	return false;
}

bool Cache_WriteData(CacheData_s* cache_data, uint32_t address, uint32_t data) 
{
	CacheAddess_s addr;
	addr.address = address;

	// check if addresss tag is locate on block_number
	if (cache_data->tsram[addr.fields.index].fields.tag == addr.fields.tag)
	{
		// hit on cache, write data
		uint16_t index = addr.fields.index * 4 + addr.fields.offset;
		cache_data->dsram[index] = data;

		return true;
	}
	// we had a miss.
	// we need to take the data from the main memory.


	return false;
}


/************************************
* static implementation             *
************************************/
static uint16_t get_block_number(CacheAddess_s addr)
{
	return addr.address / BLOCK_SIZE;
}

