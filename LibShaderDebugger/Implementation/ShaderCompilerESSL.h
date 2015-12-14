/*******************************************************************************\
*																				*
*			Class   : ShaderCompilerESSL				                            *
*			Purpose : Compiler a shader to intermediate node tree or object     *
*                     code for WebGL 1.0 GLSL dialect                           *
*			File    : ShaderCompilerESSL.h							            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __SHADER_COMPILER_ESSL_H__
#define __SHADER_COMPILER_ESSL_H__


#include "SmartPointerDeclarations.h"


// ANGLE headers
#include "compiler/translator/TranslatorESSL.h"


class ShaderCompilerESSL : public TranslatorESSL
{
public:
                                // Constructor
                                ShaderCompilerESSL(sh::GLenum inType, ShShaderSpec inSpec);

                                // Compile source to node tree representation
    TIntermNode*                CompileToAST(const std::string& inSource, int inCompileOptions);

                                // Compile node tree representation to object code
    bool                        CompileFromAST(TIntermNode* inRootAST, int inCompileOptions);
};


#endif // __SHADER_COMPILER_ESSL_H__
