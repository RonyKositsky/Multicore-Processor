#define _CRT_SECURE_NO_WARNINGS
/*!
******************************************************************************
\file MainMemory.c
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
#include "../include/MainMemory.h"
#include "../include/Files.h"
//
#include <string.h>

/************************************
*      definitions                 *
************************************/

/************************************
*       types                       *
************************************/

/************************************
*      variables                    *
************************************/
static uint32_t gMemory[MAIN_MEMORY_SIZE];

/************************************
*      static functions             *
************************************/

/************************************
*       API implementation          *
************************************/
void MainMemory_Init(void)
{
	memset((uint8_t*)gMemory, 0, sizeof(gMemory));

	uint16_t lineInProgram = 0;
	while (lineInProgram < MAIN_MEMORY_SIZE && fscanf(MeminFile, "%08x", (uint32_t*)&(gMemory[lineInProgram])) != EOF)
		lineInProgram++;
}

uint32_t MainMemory_ReadValue(uint32_t index)
{
	if (index < MAIN_MEMORY_SIZE)
		return gMemory[index];

	return NULL;
}

void MainMemory_WriteValue(uint32_t index, uint32_t data)
{
	if (index < MAIN_MEMORY_SIZE)
		gMemory[index] = data;
}

/************************************
* static implementation             *
************************************/

