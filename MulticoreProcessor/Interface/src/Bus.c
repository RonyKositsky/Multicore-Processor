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
#include "../include/Files.h"
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

typedef enum
{
	transaction_idle_state,
	transaction_wait_state,
	transaction_start_operation_state,
	transaction_operation_state,
	transaction_finally_state,
} transaction_state_e;

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
//
// Callbacks
static shared_signal_callback	gSharedSignalCallback;
static cache_snooping_callback	gCacheSnoopingCallback;
static cache_response_callback	gCacheResponseCallback;
static memory_callback_t		gMemoryCallback;
//
// global variables
static bool gBusInProgress;
static transaction_state_e gBusTransactionState[NUMBER_OF_CORES] = { 0, 0, 0, 0 };
static Bus_packet_s gCurrentPacket;
static uint8_t gAddressOffset;
static uint32_t gIterCounter = 0;

// Fifo variables
static queue_item_s* gQueueHead;
static queue_item_s* gQueueTail;

/************************************
*      static functions             *
************************************/
static bool check_shared_line(Bus_packet_s* packet, bool* is_modified);
static bool check_cache_snooping(Bus_packet_s* packet);
static void print_bus_status(Bus_packet_s packet);

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

void Bus_RegisterCacheCallbacks(shared_signal_callback signal_callback,
								cache_snooping_callback snooping_callback,
								cache_response_callback response_callback)
{
	gSharedSignalCallback = signal_callback;
	gCacheSnoopingCallback = snooping_callback;
	gCacheResponseCallback = response_callback;
}


void Bus_RegisterMemoryCallback(memory_callback_t callback)
{
	gMemoryCallback = callback;
}

void Bus_AddTransaction(Bus_packet_s packet)
{
	// check if this a duplicate transaction
	bus_fifo_Enqueue(packet);
	gBusTransactionState[packet.bus_origid] = transaction_wait_state;
}

bool Bus_InTransaction(Bus_originator_e originator)
{
	return gBusTransactionState[originator] != transaction_idle_state &&
		gBusTransactionState[originator] != transaction_start_operation_state;
}

bool Bus_WaitForTransaction(Bus_originator_e originator)
{
	return gBusTransactionState[originator] == transaction_wait_state;
}

void Bus_Iter(void)
{
	static bool is_first_shared = true;
	gIterCounter++;
		
	if (gBusTransactionState[gCurrentPacket.bus_origid] == transaction_finally_state)
		gBusTransactionState[gCurrentPacket.bus_origid] = transaction_idle_state;
	else if (gBusTransactionState[gCurrentPacket.bus_origid] == transaction_start_operation_state)
		gBusTransactionState[gCurrentPacket.bus_origid] = transaction_operation_state;


	if (bus_fifo_IsEmpty() && !gBusInProgress)
	{
		gCurrentPacket.bus_origid = bus_invalid_originator;
		return;
	}


	if (!gBusInProgress)
	{
		is_first_shared = true;
		int prev_origid = gCurrentPacket.bus_origid;
		
		if (!bus_fifo_Dequeue(&gCurrentPacket) || gCurrentPacket.bus_origid == bus_invalid_originator)
			return;

		gCurrentPacket.bus_original_sender = gCurrentPacket.bus_origid;

		gBusInProgress = true;
		gBusTransactionState[gCurrentPacket.bus_origid] = transaction_operation_state;
		gAddressOffset = 0;
		// print bus trace
		printf("bus trace - (#%d) dequeue next cmd\n", gIterCounter);
		print_bus_status(gCurrentPacket);
	}

	Bus_packet_s packet;
	memcpy(&packet, &gCurrentPacket, sizeof(gCurrentPacket));

	memory_addess_s address = { .address = gCurrentPacket.bus_addr };
	address.fields.offset = gAddressOffset;
	packet.bus_addr = address.address;

	bool is_modified = false;
	packet.bus_shared = check_shared_line(&gCurrentPacket, &is_modified);
	if (is_modified && is_first_shared)
	{
		is_first_shared = false;
		return;
	}

	bool cache_response  = check_cache_snooping(&packet);
	bool memory_response = gMemoryCallback(&packet, is_modified);
	
	if (memory_response)
	{
		// print response trace.
		printf("bus trace - (#%d) response to sender\n", gIterCounter);
		print_bus_status(packet);

		// send the response packet back to the sender
		if (gCacheResponseCallback(gCacheInterface[gCurrentPacket.bus_origid].cache_data, &packet, &gAddressOffset))
		{
			gBusTransactionState[gCurrentPacket.bus_origid] = transaction_finally_state;
			gBusInProgress = false;
		}
	}
}


/************************************
* static implementation             *
************************************/
static bool check_shared_line(Bus_packet_s* packet, bool* is_modified)
{
	bool shared = false;

	for (int i = 0; i < NUMBER_OF_CORES; i++)
		shared |= gSharedSignalCallback(gCacheInterface[i].cache_data, packet, is_modified);
	
	return shared;
}

static bool check_cache_snooping(Bus_packet_s* packet)
{
	bool cache_response = false;
	for (int i = 0; i < NUMBER_OF_CORES; i++)
		cache_response |= gCacheSnoopingCallback(gCacheInterface[i].cache_data, packet, gAddressOffset);
	
	return cache_response;
}

static void print_bus_status(Bus_packet_s packet)
{
	fprintf(BusTraceFile, "%d %d %d %05X %08X %d\n", gIterCounter, packet.bus_origid, packet.bus_cmd, 
		packet.bus_addr, packet.bus_data, packet.bus_shared);
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
