/*******************************************************************************\
*																				*
*			Class   : InspectContextSingleTriangle	                            *
*			Purpose : Interface to program and all state to needed to run       *
*                     program                                                   *
*			File    : InspectContextSingleTriangle.h							    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __INSPECT_CONTEXT_SINGLE_TRIANGLE_H__
#define __INSPECT_CONTEXT_SINGLE_TRIANGLE_H__


// Project headers
#include "InspectContextWebGL1Basic.h"


class InspectContextSingleTriangle : public InspectContextWebGL1Basic
{
public:
                                // Constructor
                                InspectContextSingleTriangle();
};


#endif // __INSPECT_CONTEXT_SINGLE_TRIANGLE_H__
