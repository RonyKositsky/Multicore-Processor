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
} cache_addess_s;

typedef Cache_mesi_e(*response_state)(CacheData_s* data, Bus_packet_s* packet);
typedef Cache_mesi_e(*snooping_state)(CacheData_s* data, Bus_packet_s* packet);

/************************************
*      static functions             *
************************************/
static bool shared_signal_handle(CacheData_s* data, Bus_packet_s* packet);
static bool cache_snooping_handle(CacheData_s* data, Bus_packet_s* packet, uint8_t address_offset);
static bool cache_response_handle(CacheData_s* data, Bus_packet_s* packet, uint8_t* address_offset);
//
// state machine for response
static Cache_mesi_e mesi_response_invalid_state(CacheData_s* data, Bus_packet_s* packet);
static Cache_mesi_e mesi_response_shared_state(CacheData_s* data, Bus_packet_s* packet);
static Cache_mesi_e mesi_response_exlusive_state(CacheData_s* data, Bus_packet_s* packet);
static Cache_mesi_e mesi_response_modified_state(CacheData_s* data, Bus_packet_s* packet);
//
// state machine for snooping
static Cache_mesi_e mesi_snooping_invalid_state(CacheData_s* data, Bus_packet_s* packet);
static Cache_mesi_e mesi_snooping_shared_state(CacheData_s* data, Bus_packet_s* packet);
static Cache_mesi_e mesi_snooping_exlusive_state(CacheData_s* data, Bus_packet_s* packet);
static Cache_mesi_e mesi_snooping_modified_state(CacheData_s* data, Bus_packet_s* packet);

/************************************
*      variables                    *
************************************/
static response_state gResponseSM[cache_mesi_max_state] = {
	mesi_response_invalid_state,
	mesi_response_shared_state,
	mesi_response_exlusive_state,
	mesi_response_modified_state
};

static snooping_state gSnoopingSM[cache_mesi_max_state] = {
	mesi_snooping_invalid_state,
	mesi_snooping_shared_state,
	mesi_snooping_exlusive_state,
	mesi_snooping_modified_state
};

/************************************
*       API implementation          *
************************************/
void Cache_Init(CacheData_s* data, Cache_Id_e id)
{
	// set all cache memory to 0;
	memset((uint8_t*)data, 0, sizeof(data));
	data->id = id;
	// Register callback
	Bus_cache_interface_s cache_interface = {
		.core_id = id, .cache_data = data, 
		.shared_signal_callback = shared_signal_handle, 
		.cache_snooping_callback = cache_snooping_handle,
		.cache_response_callback = cache_response_handle };
	Bus_RegisterCache(cache_interface);
}

bool Cache_ReadData(CacheData_s* cache_data, uint32_t address, uint32_t* data)
{
	cache_addess_s addr;
	addr.address = address;

	// check if addresss tag is locate on block_number
	if (cache_data->tsram[addr.fields.index].fields.tag == addr.fields.tag) 
	{
		// hit on cache, getting the value 
		uint16_t index = addr.fields.index * BLOCK_SIZE + addr.fields.offset;
		*data = cache_data->dsram[index];

		return true;
	}
	// we had a miss.
	//
	Bus_packet_s packet;
	packet.bus_origid = cache_data->id;

	// first, check if the required block is dirty
	// if so, we need first to send the block into the memory
	if (cache_data->tsram[addr.fields.index].fields.mesi == cache_mesi_modified)
	{
		// get stored block address
		cache_addess_s block_addr = {
			.fields.index = addr.fields.index,
			.fields.tag = cache_data->tsram[addr.fields.index].fields.tag,
			.fields.offset = 0
		};

		// send bus flush
		packet.bus_cmd = bus_flush;
		packet.bus_addr = block_addr.address;

		uint16_t index = addr.fields.index * BLOCK_SIZE + addr.fields.offset;
		packet.bus_data = cache_data->dsram[index];
		
		// add the flush transaction into the bus queue
		Bus_AddTransaction(packet);
	}

	// now, we need to take the new block from the main memory.
	packet.bus_cmd = bus_busRd;
	packet.bus_addr = addr.address;

	// add the read transaction into the bus queue
	Bus_AddTransaction(packet);

	return false;
}

bool Cache_WriteData(CacheData_s* cache_data, uint32_t address, uint32_t data) 
{
	cache_addess_s addr;
	addr.address = address;
	//
	Tsram_s* tsram = &(cache_data->tsram[addr.fields.index]);
	//
 	Bus_packet_s packet;
	packet.bus_origid = cache_data->id;

	// check if addresss tag is locate on block_number
	if (tsram->fields.tag == addr.fields.tag)
	{
		// hit on cache, write data
		// if the block is exclusive ours, we just change it locally
		if (tsram->fields.mesi == cache_mesi_exclusive || tsram->fields.mesi == cache_mesi_modified)
		{
			// do nothing
		}
		else if (tsram->fields.mesi == cache_mesi_shared)
		{
			// Send Bus_Rdx to make sure this block is exclusive ours
			packet.bus_cmd = bus_busRdX;
			packet.bus_addr = addr.address;
			Bus_AddTransaction(packet);
		}

		uint16_t index = addr.fields.index * BLOCK_SIZE + addr.fields.offset;
		cache_data->dsram[index] = data;
		// update modified flag.
		cache_data->tsram[addr.fields.index].fields.mesi = cache_mesi_modified;



		return true;
	}
	// we had a miss.
	// we need to take the data from the main memory.


	return false;
}


/************************************
* static implementation             *
************************************/
static bool shared_signal_handle(CacheData_s* data, Bus_packet_s* packet)
{
	// check if this is my packet
	if (data->id == packet->bus_origid)
		return false;

	cache_addess_s address = { .address = packet->bus_addr };
	//if (packet->bus_cmd == bus_busRd)			// TODO: check if need this if statement
	//{
	//	// check if addresss tag is locate on block_number
	//	if (data->tsram[address.fields.index].fields.tag == address.fields.tag)
	//		return true;
	//}

	return data->tsram[address.fields.index].fields.tag == address.fields.tag;
}

static bool cache_snooping_handle(CacheData_s* data, Bus_packet_s* packet, uint8_t address_offset)
{
	// check if this is my packet
	if (data->id == packet->bus_origid)
		return false;

	cache_addess_s address = { .address = packet->bus_addr };
	Tsram_s* tsram = &(data->tsram[address.fields.index]);

	// check if the block is in the cache, if not, do nothing.
	if (tsram->fields.tag != address.fields.tag)
		return false;

	// execute block state machine
	Cache_mesi_e next_state = gSnoopingSM[tsram->fields.mesi](data, packet);

	if (address_offset == (BLOCK_SIZE - 1))
	{
		tsram->fields.mesi = next_state;
		return true;
	}
	
	return false;
}

static bool cache_response_handle(CacheData_s* data, Bus_packet_s* packet, uint8_t* address_offset)
{
	// check if this is my packet
	if (data->id == packet->bus_origid)
		return false;

	cache_addess_s address = { .address = packet->bus_addr };
	Tsram_s* tsram = &(data->tsram[address.fields.index]);
	// check if the block is in the cache, if not, do nothing.
	if (tsram->fields.tag != address.fields.tag)
		return false;

	// execute block state machine
	Cache_mesi_e next_state = gResponseSM[tsram->fields.mesi](data, packet);

	if (*address_offset == (BLOCK_SIZE - 1))
	{
		tsram->fields.mesi = next_state;
		return true;
	}

	*address_offset += 1;
	return false;
}

// state machine for snooping
static Cache_mesi_e mesi_snooping_invalid_state(CacheData_s* data, Bus_packet_s* packet)
{
	// todo: check the default return value
	return cache_mesi_invalid;
}

static Cache_mesi_e mesi_snooping_shared_state(CacheData_s* data, Bus_packet_s* packet)
{
	if (packet->bus_cmd == bus_busRdX)
		return cache_mesi_invalid;

	// todo: check the default return value
	return cache_mesi_shared;
}

static Cache_mesi_e mesi_snooping_exlusive_state(CacheData_s* data, Bus_packet_s* packet)
{
	if (packet->bus_cmd == bus_busRd)
		return cache_mesi_shared;

	if (packet->bus_cmd == bus_busRdX)
		return cache_mesi_invalid;

	// todo: check the default return value
	return cache_mesi_exclusive;
}

static Cache_mesi_e mesi_snooping_modified_state(CacheData_s* data, Bus_packet_s* packet)
{
	cache_addess_s address = { .address = packet->bus_addr };
	uint16_t index = address.fields.index * BLOCK_SIZE + address.fields.offset;

	if (packet->bus_cmd == bus_busRd)
	{
		// send back the modified data
		packet->bus_cmd = bus_flush;
		packet->bus_data = data->dsram[index];

		return cache_mesi_shared;
	}

	if (packet->bus_cmd == bus_busRdX)
	{
		// send back the modified data
		packet->bus_cmd = bus_flush;
		packet->bus_data = data->dsram[index];

		return cache_mesi_invalid;
	}

	// todo: check the default return value
	return cache_mesi_modified;
}

// state machine for response
static Cache_mesi_e mesi_response_invalid_state(CacheData_s* data, Bus_packet_s* packet)
{
	if (packet->bus_cmd == bus_busRd)
		return packet->bus_shared ? cache_mesi_shared : cache_mesi_exclusive;
	
	if (packet->bus_cmd == bus_busRdX)
		return cache_mesi_modified;
	
	// todo: check the default return value
	return cache_mesi_invalid;
}

static Cache_mesi_e mesi_response_shared_state(CacheData_s* data, Bus_packet_s* packet)
{
	if (packet->bus_cmd == bus_busRdX)
		return cache_mesi_modified;

	// todo: check the default return value
	return cache_mesi_shared;
}

static Cache_mesi_e mesi_response_exlusive_state(CacheData_s* data, Bus_packet_s* packet)
{
	// todo: check the default return value
	return cache_mesi_exclusive;
}

static Cache_mesi_e mesi_response_modified_state(CacheData_s* data, Bus_packet_s* packet)
{
	// hadle flush
	if (packet->bus_cmd == bus_flush)
	{
		// change packet data to the next item in the block
		cache_addess_s address = { .address = packet->bus_addr };
		address.fields.offset++;

		uint16_t index = address.fields.index * BLOCK_SIZE + address.fields.offset;
		packet->bus_data = data->dsram[index];
		return cache_mesi_invalid;
	}

	// todo: check the default return value
	return cache_mesi_modified;
}

