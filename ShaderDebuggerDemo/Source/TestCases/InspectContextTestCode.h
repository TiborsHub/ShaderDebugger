/*******************************************************************************\
*																				*
*			Class   : InspectContextTestCode	                                    *
*			Purpose : Interface to program and all state needed to run          *
*                     program to test code                                      *
*			File    : InspectContextTestCode.h							        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __INSPECT_CONTEXT_TEST_CODE_H__
#define __INSPECT_CONTEXT_TEST_CODE_H__


// Project headers
#include "InspectContextWebGL1Basic.h"


class InspectContextTestCode : public InspectContextWebGL1Basic
{
public:
                                // Constructor
                                InspectContextTestCode();
};


#endif // __INSPECT_CONTEXT_TEST_CODE_H__
