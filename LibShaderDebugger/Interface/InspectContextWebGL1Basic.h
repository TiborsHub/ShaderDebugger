/*******************************************************************************\
*                                                                               *
*           Class   : InspectContextWebGL1Basic                                 *
*           Purpose : Inspect context with default functionality to render      *
*                     one vertex buffer and update the viewport                 *
*           File    : InspectContextWebGL1Basic.h                               *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
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
                                InspectContextWebGL1Basic(
                                    const std::string&          inSourceVertex,
                                    const std::string&          inSourceFragment,
                                    const std::vector<GLfloat>& inVertices,
                                    GLenum                      inVertexListType);

                                // Run the program which is the subject of the inspection
    virtual bool                RunTargetProgram() override;

protected:
    std::vector<GLfloat>        mVertexList;
    GLenum                      mVertexListType;
};


#endif // __INSPECT_CONTEXT_WEBGL_1_BASIC_H__
