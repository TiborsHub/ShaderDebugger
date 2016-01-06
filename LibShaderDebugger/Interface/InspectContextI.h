/*******************************************************************************\
*                                                                               *
*           Class   : InspectContext    I                                       *
*           Purpose : Interface to program and all state needed to run          *
*                     program                                                   *
*           File    : InspectContext    I.h                                     *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __INSPECT_CONTEXT_I_H__
#define __INSPECT_CONTEXT_I_H__


// Forward declarations
class Win32EmbeddedWindow;


// Standard headers
#include <string>
#include <cstdint>


// ANGLE headers
#include "GLSLANG\ShaderLang.h"


class InspectContextI
{
public:
                                // Destructor
    virtual                     ~InspectContextI() {};

                                // Initialize context
    virtual bool                Initialize(std::string& outText) = 0;

                                // Return count of program parts
    virtual size_t              GetShaderCount() const = 0;

                                // Return type of
    virtual uint64_t            GetShaderType(size_t inShaderIx) const = 0;

                                // Return source of shader
    virtual std::string         GetShaderSource(size_t inShaderIx) const = 0;

                                // Return handle of shader
    virtual uint64_t            GetHandle(size_t inShaderIx) = 0;

                                // Activate the shader program
    virtual bool                UseProgram() = 0;

                                // Run the program which is the subject of the inspection
    virtual bool                RunTargetProgram() = 0;
};


#endif // __INSPECT_CONTEXT_I_H__
