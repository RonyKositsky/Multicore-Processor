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
typedef struct
{	
	uint8_t bus_shared;
	uint8_t bus_origid;
	uint8_t bus_cmd;
	uint32_t bus_addr;
	uint32_t bus_addr;
	uint32_t bus_data;

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
Initialize func

\details
Must be called only once

\param
 [in] counter_val - reset counter value
 [out] out_val    -

\return none
*****************************************************************************/


#endif //__FILE_NAME_H__