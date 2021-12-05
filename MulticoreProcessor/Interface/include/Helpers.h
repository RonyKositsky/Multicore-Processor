/*!
******************************************************************************
\file file_name.h
\date 17 October 2021
\author Rony Kosistky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan Gartenberg
\par
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef __HELPERS_H__
#define __HELPERS_H__

/************************************
*      include                      *
************************************/
#include <stdint.h>
#include <stdbool.h>

/************************************
*      definitions                 *
************************************/
#define NUMBER_OF_CORES 4
#define NUMBER_OF_REGISTERS 16
#define IMMEDIATE_REGISTER_INDEX 1
#define ZERO_REGISTER_INDEX 0
#define START_MUTABLE_REGISTER_INDEX 2
#define PROGRAM_COUNTER_REGISTER_NUM 15

/************************************
*       types                       *
************************************/
typedef union
{
	struct
	{
		uint16_t immediate : 12;	// [0:11]  Immediate value
		uint16_t rt : 4;			// [12:15] src1 value
		uint16_t rs : 4;			// [16:19] src0 value
		uint16_t rd : 4;			// [20:23] src0 value
		uint16_t opcode : 8;		// [24:31] opcode value
	} bits;

	uint32_t command;
} InstructionFormat_s;

#endif //__FILE_NAME_H__









