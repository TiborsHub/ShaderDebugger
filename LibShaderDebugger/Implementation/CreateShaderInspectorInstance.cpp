/*******************************************************************************\
*																				*
*			Utility : CreateShaderInspectorInstance					            *
*			Purpose : Factory function for creating shader inspectors           *
*			File    : CreateShaderInspectorInstance.cpp							*
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "CreateShaderInspectorInstance.h"


// Library headers
#include "ShaderInspectorWebGL.h"


// Return Shader inspector instance
SpShaderInspectorI
CreateShaderInspectorInstance(
    int                inShaderIx,
    ProgramInspectorI* inProgramInspector,
    uint64_t           inShaderType)
{
    return std::make_shared<ShaderInspectorWebGL>(
        inShaderIx,
        inProgramInspector,
        sh::GLenum(inShaderType),
        SH_WEBGL_SPEC,
        SH_HLSL11_OUTPUT,
        SH_VALIDATE);
}

