/*******************************************************************************\
*                                                                               *
*           Utility : CreateShaderInspectorInstance                             *
*           Purpose : Factory function for creating shader inspectors           *
*           File    : CreateShaderInspectorInstance.h                           *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __CREATE_SHADER_INSPECTOR_INSTANCE_H__
#define __CREATE_SHADER_INSPECTOR_INSTANCE_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(ShaderInspectorI);
CLASS_FORWARD_DECLARATIONS(ProgramInspectorI);


// Return Shader inspector instance
SpShaderInspectorI
CreateShaderInspectorInstance(
    int                inShaderIx,
    ProgramInspectorI* inProgramInspector,
    uint64_t           inShaderType);


#endif // __CREATE_SHADER_INSPECTOR_INSTANCE_H__
