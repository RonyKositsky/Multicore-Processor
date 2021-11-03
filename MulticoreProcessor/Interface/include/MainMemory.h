/*!
******************************************************************************
\file MainMemory.h
\date 17 October 2021
\author Rony Kosistky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan Gartenberg
\par
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef __MAIN_MEMORY_H__
#define __MAIN_MEMORY_H__

/************************************
*      include                      *
************************************/
#include <stdint.h>

/************************************
*      definitions                 *
************************************/
#define MAIN_MEMORY_SIZE		(0x100000)		// (1 << 20) = 2^20

/************************************
*       types                       *
************************************/


/************************************
*       API                         *
************************************/
/*!
******************************************************************************
\brief
Initialize main memory from input file.

\details
Must be called only once

\return none
*****************************************************************************/
void MainMemory_Init(void);

/*!
******************************************************************************
\brief
 Read value from memory

\details
 read from specific index.

\param
 [in] index - memory index.

\return memory value at index.
*****************************************************************************/
uint32_t MainMemory_ReadValue(uint32_t index);

/*!
******************************************************************************
\brief
 Write value to memory

\details
 write to specific index at memory.

\param
 [in] index - memory index.
 [in] data - data to store.

\return memory value at index.
*****************************************************************************/
void MainMemory_WriteValue(uint32_t index, uint32_t data);

#endif //__MAIN_MEMORY_H__