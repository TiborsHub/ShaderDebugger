/*******************************************************************************\
*																				*
*			Class   : InspectContextValueTest	                                *
*			Purpose : Test reading of color channel from the framebuffer        *
*			File    : InspectContextValueTest.h							        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __INSPECT_CONTEXT_COLOR_TEST_H__
#define __INSPECT_CONTEXT_COLOR_TEST_H__


// Project headers
#include "InspectContextWebGL1Basic.h"


class InspectContextValueTest : public InspectContextWebGL1Basic
{
public:
                                // Constructor
                                InspectContextValueTest(const std::string& inColorValue);
};


#endif // __INSPECT_CONTEXT_COLOR_TEST_H__
