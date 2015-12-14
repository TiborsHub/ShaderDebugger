/*******************************************************************************\
*																				*
*			Class   : InspectContextWebGL1Basic	                                *
*			Purpose : Inspect context with default functionality to render      *
*                     one vertex buffer and update the viewport                 *
*			File    : InspectContextWebGL1Basic.h							    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __INSPECT_CONTEXT_WEBGL_1_BASIC_H__
#define __INSPECT_CONTEXT_WEBGL_1_BASIC_H__


// Project headers
#include "InspectContextWebGL1.h"


class InspectContextWebGL1Basic : public InspectContextWebGL1
{
public:
                                // Constructor
                                InspectContextWebGL1Basic();

                                // Run the program which is the subject of the inspection
    virtual bool                RunTargetProgram() override;

protected:
    std::vector<GLfloat>        mVertices;
};


#endif // __INSPECT_CONTEXT_WEBGL_1_BASIC_H__
