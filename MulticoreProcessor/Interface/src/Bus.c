/*!
******************************************************************************
\file Bus.c
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
#include "../include/Bus.h"
#include "../include/Helpers.h"
//
#include <string.h>
#include <stdlib.h>

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

// Fifo types
typedef struct _queue_item_s
{
	Bus_packet_s item;
	struct _queue_item_s* parent;
	struct _queue_item_s* next;
} queue_item_s;

/************************************
*      variables                    *
************************************/
static Bus_cache_interface_s gCacheInterface[NUMBER_OF_CORES];
static memory_callback_t gMemoryCallback;
static bool gBusInProgress;
static Bus_packet_s gCurrentPacket;
static uint8_t gAddressOffset;

// Fifo variables
static queue_item_s* gQueueHead;
static queue_item_s* gQueueTail;

/************************************
*      static functions             *
************************************/
static bool check_shared_line(Bus_packet_s* packet);
static bool check_cache_snooping(Bus_packet_s* packet);


// Fifo functions
bool bus_fifo_IsEmpty(void);
bool bus_fifo_Enqueue(Bus_packet_s item);
bool bus_fifo_Dequeue(Bus_packet_s* item);

/************************************
*       API implementation          *
************************************/
void Bus_RegisterCache(Bus_cache_interface_s cache_interface)
{
	gCacheInterface[cache_interface.core_id] = cache_interface;
}

void Bus_RegisterMemoryCallback(memory_callback_t callback)
{
	gMemoryCallback = callback;
}

void Bus_AddTransaction(Bus_packet_s packet)
{
	bus_fifo_Enqueue(packet);
}

void Bus_Itereration(void)
{
	if (bus_fifo_IsEmpty() && !gBusInProgress)
		return;

	if (!gBusInProgress)
	{
		if (!bus_fifo_Dequeue(&gCurrentPacket))
			return;

		gBusInProgress = true;
		gAddressOffset = 0;
		// print bus trace
		
	}

	Bus_packet_s packet;
	memcpy(&packet, &gCurrentPacket, sizeof(gCurrentPacket));
	//
	memory_addess_s address = { .address = gCurrentPacket.bus_addr };
	address.fields.offset = gAddressOffset;
	packet.bus_addr = address.address;
	//
	packet.bus_shared = check_shared_line(&gCurrentPacket);
	//
	bool cache_response = check_cache_snooping(&packet);
	bool memory_response = gMemoryCallback(&packet, cache_response);

	if (cache_response || memory_response)
	{
		// print response trace.

		// send the response packet back to the sender
		if (gCacheInterface[gCurrentPacket.bus_origid].cache_response_callback(gCacheInterface[gCurrentPacket.bus_origid].cache_data, &packet, &gAddressOffset))
			gBusInProgress = false;
	}
}


/************************************
* static implementation             *
************************************/
static bool check_shared_line(Bus_packet_s* packet)
{
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		if (gCacheInterface[i].shared_signal_callback(gCacheInterface[i].cache_data, packet))
			return true;
	}
	return false;
}

static bool check_cache_snooping(Bus_packet_s* packet)
{
	if (!packet->bus_shared)
		return false;

	bool cache_response = false;
	for (int i = 0; i < NUMBER_OF_CORES; i++)
		cache_response |= gCacheInterface[i].cache_snooping_callback(gCacheInterface[i].cache_data, packet, gAddressOffset);
	
	return cache_response;
}


// Fifo implemantation
bool bus_fifo_IsEmpty(void)
{
	return gQueueHead == NULL;
}

bool bus_fifo_Enqueue(Bus_packet_s item)
{
	queue_item_s* queue_item = malloc(sizeof(queue_item_s));
	if (queue_item == NULL)
		return false;

	// initiate item
	queue_item->item = item;
	queue_item->next = NULL;
	queue_item->parent = NULL;

	if (bus_fifo_IsEmpty())
	{
		gQueueHead = queue_item;
		gQueueTail = queue_item;
	}
	else
	{
		gQueueHead->parent = queue_item;
		queue_item->next = gQueueHead;
		gQueueHead = queue_item;
	}
	return true;
}

bool bus_fifo_Dequeue(Bus_packet_s* item)
{
	if (gQueueTail == NULL)
		return false;

	queue_item_s* queue_item = gQueueTail;
	gQueueTail = queue_item->parent;

	if (gQueueTail == NULL)
		gQueueHead = NULL;
	else
		gQueueTail->next = NULL;

	queue_item->parent = NULL;
	*item = queue_item->item;

	free(queue_item);
	return true;
}
