/*!
******************************************************************************
\file Cache.h
\date 30 October 2021
\author Rony Kosistky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan Gartenberg
\par
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef __CACHE_H__
#define __CACHE_H__

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
void Cache_Init(void);

/*!
******************************************************************************
\brief
 Get data from cache

\details
 if address is not match the cache will take it from the main memory.

\param
 [in] address - the memory address.
 [out] data   - the memory at the recieved address.

\return true if address exist on cache.
*****************************************************************************/
bool Cache_GetData(uint32_t address, uint32_t* data);

#endif // __CACHE_H__