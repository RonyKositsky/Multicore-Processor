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

#endif //__MAIN_MEMORY_H__