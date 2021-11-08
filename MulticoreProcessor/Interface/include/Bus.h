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

/************************************
*      definitions                 *
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
	uint8_t bus_shared;
}Bus_wires_s;
/************************************
*       types                       *
************************************/


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
void Bus_AddTransaction(Bus_wires_s wire);

/*!
******************************************************************************
\brief
 Bus iteration

\return none
*****************************************************************************/
void Bus_Iter(void);

#endif //__BUS_H__