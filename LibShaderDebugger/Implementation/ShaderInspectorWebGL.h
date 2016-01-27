/*******************************************************************************\
*                                                                               *
*           Class   : ShaderInspectorWebGL                                      *
*           Purpose : Inspector of a shader using the ANGLE library             *
*           File    : ShaderInspectorWebGL.h                                    *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __SHADER_INSPECTOR_ANGLE_H__
#define __SHADER_INSPECTOR_ANGLE_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
class InspectResult;
class TIntermNode;
class TIntermSymbol;
CLASS_FORWARD_DECLARATIONS(ShaderCompilerESSL);
CLASS_FORWARD_DECLARATIONS(TParseContext);
CLASS_FORWARD_DECLARATIONS(DataFramebuffer);
CLASS_FORWARD_DECLARATIONS(ShaderStructureNodes);


// Library headers
#include "ShaderInspectorI.h"


// Standard headers
#include <string>
#include <cstdint>


// ANGLE headers
#include "GLES2/gl2.h"
#include "GLSLANG/ShaderLang.h"
#include "compiler/translator/InfoSink.h"
#include "compiler/translator/Intermediate.h"


class ShaderInspectorWebGL : public ShaderInspectorI
{
public:
                                // Constructor
                                ShaderInspectorWebGL(
                                    int                inShaderIx,
                                    ProgramInspectorI* inProgramInspector,
                                    sh::GLenum         inShaderType,
                                    ShShaderSpec       inShaderSpec,
                                    ShShaderOutput     inOutputLanguage,
                                    ShCompileOptions   inCompileOptions);

                                // Initialize the inspector
    virtual bool                Initialize();

                                // Returns information about the ast where the shader execution path can branch
                                // and other information necessary to keep track of the state of the shader
    virtual bool                GetShaderStructure(ShaderStructureState& outShaderStructure) override;

                                // Return location in source code for an ast node
    virtual bool                GetSourceLocation(
                                    const tASTLocation& inASTLocation,
                                    SourceLocation&     outSourceLocation) override;

                                // Return next statement
    virtual bool                GetNextStatement(ShaderStructureState& ioCurrentState) override;

                                // Inspect a token at the given source position
    virtual void                Inspect(
                                    const std::string& inSymbol,
                                    size_t             inLine,
                                    size_t             inColumn,
                                    InspectResult&     outResult) override;

private:
                                // Return the nodes from the ast where the shader execution path can branch
    bool                        GetShaderStructureNodes(ShaderStructureNodes& outShaderStructure);

                                // Returns true if node is a statement for the debugger to halt on
    bool                        IsDebugStepStatement(TIntermNode* inNode);

                                // Get next ast node after return from function call
                                // Input is callstack pointing to function call which returned
                                // The node location inside the function is just popped off the stack
    void                        GetNextNodeAfterFunctionCall(std::vector<tASTNodeLocation>& ioCallStack);

                                // Get next node from ast which is the next statement to execute
    void                        GetNextDebugStepNode(std::vector<tASTNodeLocation>& ioCallStack);

                                // Transform AST to return target symbol
    void                        TransformAST(
                                    const tASTNodeLocation& inTargetSymbolPath,
                                    InspectResult&          outResult);

                                // Returns node which receives the output value
    TIntermTyped*               CreateOutputVariableNode();

                                // Run modified shader in context of original program
    void                        RunInspectProgram(
                                    const std::string& inInspectShaderSrc,
                                    InspectResult&     outResult);

    sh::GLenum                  mShaderType;
    ShShaderSpec                mShaderSpec;
    ShShaderOutput              mOutputLanguage;
    ShCompileOptions            mCompileOptions;
    TInfoSink                   mInfoSink;
    TIntermediate               mNodeFactory;
    UpShaderCompilerESSL        mCompiler;
    UpTParseContext             mParseContext; // For generating statements as node trees
    ShBuiltInResources          mResources;
    UpDataFramebuffer           mDataReader;
    UpShaderStructureNodes      mStructureNodes;
};


#endif // __SHADER_INSPECTOR_ANGLE_H__
