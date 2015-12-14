/*******************************************************************************\
*																				*
*			Utility : LibShaderDebuggerInitFin					                *
*			Purpose : Utility functions to initialize and finalize the          *
*                     shader debugger library                                   *
*			File    : LibShaderDebuggerInitFin.cpp							    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "LibShaderDebuggerInitFin.h"


// ANGLE headers
#include "GLSLANG/ShaderLang.h"
#include "compiler/translator/Compiler.h"


// Standard headers
#include <assert.h>


static_assert(ANGLE_ENABLE_HLSL, "ANGLE_ENABLE_HLSL not defined");


namespace
{
    bool is_initialized(false);
}


// Initialize the shader debugger library
bool
LibShaderDebuggerInitialize()
{
    assert(&TShHandleBase::getAsTranslatorHLSL != nullptr);

    is_initialized = ShInitialize();
    return is_initialized;
}


// Finalize the shader debugger library
bool
LibShaderDebuggerFinalize()
{
    assert(is_initialized);

    return ShFinalize();
}

