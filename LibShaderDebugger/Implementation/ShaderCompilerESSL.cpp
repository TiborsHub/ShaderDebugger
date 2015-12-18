/*******************************************************************************\
*																				*
*			Class   : ShaderCompilerESSL				                            *
*			Purpose : Compiler a shader to intermediate node tree or object     *
*                     code for WebGL 1.0 GLSL dialect                           *
*			File    : ShaderCompilerESSL.cpp							            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "ShaderCompilerESSL.h"


// Constructor
ShaderCompilerESSL::ShaderCompilerESSL(sh::GLenum inType, ShShaderSpec inSpec) :
    TranslatorESSL(inType, inSpec)
{
 
}


// Compile source to node tree representation
TIntermNode*
ShaderCompilerESSL::CompileToAST(const std::string& inSource, int inCompileOptions)
{
    const char* source_arr[1] = { inSource.c_str() };

    // Mark memory
    allocator.push();
    SetGlobalPoolAllocator(&allocator);

    inCompileOptions |= SH_INTERMEDIATE_TREE;
    return compileTreeForTesting(source_arr, 1, inCompileOptions);
}


// Compile node tree representation to object code
bool
ShaderCompilerESSL::CompileFromAST(TIntermNode* inRootAST, int inCompileOptions)
{
    assert(GetGlobalPoolAllocator() == &allocator);
    translate(inRootAST, inCompileOptions);

    // Release memory
    // All allocated nodes and intermediate objects become invalid
    assert(GetGlobalPoolAllocator() == &allocator);
    SetGlobalPoolAllocator(nullptr);
    allocator.pop();

    return true;
}
