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
typedef enum
{
	cache_core0,
	cache_core1,
	cache_core2,
	cache_core3,
} Cache_Id_e;

typedef enum 
{
	cache_mesi_invalid,
	cache_mesi_shared,
	cache_mesi_exclusive,
	cache_mesi_modified,
	//
	cache_mesi_max_state
} Cache_mesi_e;

typedef union
{
	uint16_t data;

	struct
	{
		uint16_t tag : 12;	// [0:11]
		Cache_mesi_e mesi : 2;	// [12:13]
	}fields;
} Tsram_s;

typedef struct
{
	Cache_Id_e id;
	bool memory_stall;
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
void Cache_Init(CacheData_s* data, Cache_Id_e id);

/*!
******************************************************************************
\brief
Register cache callback handles to the bus.

\details
Must be called only once, after initialization

\return none
*****************************************************************************/
void Cache_RegisterBusHandles(void);

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