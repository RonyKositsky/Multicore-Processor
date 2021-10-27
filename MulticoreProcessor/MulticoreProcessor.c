/*!
******************************************************************************
\file
\date 26 October 2021
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
#include "Helpers/include/Files.h"

/************************************
*      definitions                  *
************************************/

/************************************
*      variables                  *
************************************/
static Core cores[NUMBER_OF_CORES];

/************************************
*      functions					*
************************************/
void CoresInit()
{
	for (int i = 0; i < NUMBER_OF_CORES; i++)
	{
		InitCore(&cores[i]);
	}

	AssignFiles(cores);
}

/************************************
*      Main							*
************************************/
int main(int argc, char *argv[])
{
	OpenFiles(argv);

	CloseFiles();
	return 0;
} 

