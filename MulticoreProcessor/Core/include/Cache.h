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
#define CACHE_SIZE	256
#define BLOCK_SIZE	4
#define FRAME_SIZE	(CACHE_SIZE / BLOCK_SIZE)

/************************************
*       types                       *
************************************/
typedef union
{
	uint16_t data;

	struct
	{
		uint16_t tag : 12;	// [0:11]
		uint16_t mesi : 2;	// [12:13]
	}fields;
} Tsram_s;

typedef struct
{
	uint32_t dsram[CACHE_SIZE];
	Tsram_s tsram[FRAME_SIZE];
} CacheData_s;

/************************************
*       API                         *
************************************/
/*!
******************************************************************************
\brief
Initialize caching

\details
Must be called only once

\param
 [in] cache_data - the cache data of specific core.

\return none
*****************************************************************************/
void Cache_Init(CacheData_s* cache_data);

/*!
******************************************************************************
\brief
 Read data from cache

\details
 if address is not match the cache will take it from the main memory.

\param
 [in] cache_data - the cache data of specific core.
 [in] address - the memory address.
 [out] data   - the memory at the recieved address.

\return true if address exist on cache.
*****************************************************************************/
bool Cache_ReadData(CacheData_s* cache_data, uint32_t address, uint32_t* data);

/*!
******************************************************************************
\brief
 Write data from cache

\details
 if address is not match the cache will take it from the main memory.

\param
 [in] cache_data - the cache data of specific core.
 [in] address - the memory address.
 [in] data   - the data to write.

\return true if address exist on cache.
*****************************************************************************/
bool Cache_WriteData(CacheData_s* cache_data, uint32_t address, uint32_t data);

#endif // __CACHE_H__