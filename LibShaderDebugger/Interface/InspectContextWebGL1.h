/*******************************************************************************\
*																				*
*			Class   : InspectContextWebGL1	                                    *
*			Purpose : Interface to program and all state needed to run a        *
*                     WebGL 1 shader program                                    *
*			File    : InspectContextWebGL1.h							            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __INSPECT_CONTEXT_WEBGL_1_H__
#define __INSPECT_CONTEXT_WEBGL_1_H__


// Project headers
#include "InspectContextI.h"


// ANGLE headers
#include <GLES2/gl2.h>


class InspectContextWebGL1 : public InspectContextI
{
public:
                                // Constructor
                                InspectContextWebGL1();

                                // Destructor
    virtual                     ~InspectContextWebGL1() override;

                                // Initialize context
    virtual bool                Initialize(std::string& outText) override;

                                // Return count of program parts
    virtual size_t              GetShaderCount() const override;

                                // Return type of
    virtual uint64_t            GetShaderType(size_t inShaderIx) const override;

                                // Return source of shader
    virtual std::string         GetShaderSource(size_t inShaderIx) const override;

                                // Return handle of shader
    virtual uint64_t            GetHandle(size_t inShaderIx) override;

                                // Activate the shader program
    virtual bool                UseProgram() override;

protected:
    GLuint                      mHandleProgram;
    GLuint                      mHandleVertex;
    GLuint                      mHandleFragment;
    std::string                 mSourceVertex;
    std::string                 mSourceFragment;
};


#endif // __INSPECT_CONTEXT_WEBGL_1_H__
