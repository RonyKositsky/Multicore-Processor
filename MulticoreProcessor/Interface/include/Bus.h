/*!
******************************************************************************
\file Bus.h
\date 17 October 2021
\author Rony Kosistky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan Gartenberg
\par
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef __BUS_H__
#define __BUS_H__

/************************************
*      include                      *
************************************/
#include <stdint.h>
#include <stdbool.h>

/************************************
*      definitions                 *
************************************/

/************************************
*       types                       *
************************************/
typedef enum
{
	bus_core0,
	bus_core1,
	bus_core2,
	bus_core3,
	bus_main_memory
} Bus_originator_e;

typedef enum
{
	bus_no_command,
	bus_busRd,
	bus_busRdX,
	bus_flush
} Bus_command_s;

typedef struct
{
	Bus_originator_e bus_origid;
	Bus_command_s bus_cmd;
	uint32_t bus_addr;
	uint32_t bus_data;
	bool bus_shared;
} Bus_packet_s;

typedef struct
{
	int core_id;
	void* cache_data;
	bool (*shared_signal_callback)(void* cache_data, Bus_packet_s* packet);
	bool (*cache_snooping_callback)(void* cache_data, Bus_packet_s* packet, uint8_t address_offset);
	bool (*cache_response_callback)(void* cache_data, Bus_packet_s* packet, uint8_t* address_offset);
} Bus_cache_interface_s;

typedef bool (*memory_callback_t)(Bus_packet_s* packet, bool direct_transaction);

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
 Initialize bus

\details
 Must be called only once

\param
 [in] counter_val - reset counter value
 [out] out_val    -

\return none
*****************************************************************************/
void Bus_Init(void);

void Bus_RegisterCache(Bus_cache_interface_s cache_interface);
void Bus_RegisterMemoryCallback(memory_callback_t callback);

/*!
******************************************************************************
\brief
 Create a new transaction on the bus.

\details
 this function add the transaction into the round-robin queue.

\param
 [in] counter_val - reset counter value
 [out] out_val    -

\return none
*****************************************************************************/
void Bus_AddTransaction(Bus_packet_s packet);

/*!
******************************************************************************
\brief
 Bus iteration

\return none
*****************************************************************************/
void Bus_Itereration(void);

#endif //__BUS_H__