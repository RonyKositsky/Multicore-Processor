/*!
******************************************************************************
\file Pipeline.h
\date 30 October 2021
\author Rony Kosistky & Ofir Guthman & Yonatan Gartenberg
\brief

\details

\par Copyright
(c) Copyright 2021 Ofir & Rony & Yonatan Gartenberg
\par
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef __PIPELINE_H__
#define __PIPELINE_H__

/************************************
*      include                      *
************************************/
#include <stdint.h>

/************************************
*      definitions                 *
************************************/
#define PIPELINE_SIZE	5

/************************************
*       types                       *
************************************/
typedef enum Pipeline_Enum
{
	FETCH,
	DECODE,
	EXECUTE,
	MEM,
	WRITE_BACK
} PipelineSM_s;

typedef struct
{
	enum Pipeline_Enum state;
	uint16_t pc;

} PipelineStage_s;

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
void Pipeline_Init(void);

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
void Pipeline_Execute(void);

#endif //__PIPELINE_H__