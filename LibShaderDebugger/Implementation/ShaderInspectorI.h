/*******************************************************************************\
*                                                                               *
*           Class   : ShaderInspectorI                                          *
*           Purpose : Interface to inspector shader                             *
*           File    : ShaderInspectorI.h                                        *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __SHADER_INSPECTOR_I_H__
#define __SHADER_INSPECTOR_I_H__


#include "SmartPointerDeclarations.h"


// Forward declaration
class InspectResult;
class ShaderStructureNodes;
class SourceLocation;
CLASS_FORWARD_DECLARATIONS(ProgramInspectorI);
CLASS_FORWARD_DECLARATIONS(InspectContextI);


// Library headers
#include "ASTLocation.h"
#include "ASTNodeLocation.h"


// Standard headers
#include <string>


class ShaderInspectorI
{
public:
                                // Constructor
                                ShaderInspectorI(int inShaderIx, ProgramInspectorI* inProgramInspector);

                                // Destructor
    virtual                     ~ShaderInspectorI() {};

                                // Initialize the inspector
    virtual bool                Initialize() = 0;

                                // Return the nodes from the ast where the shader execution path can branch
    virtual bool                GetShaderStructureNodes(ShaderStructureNodes& outShaderStructure) = 0;

                                // Return location in source code for an ast node
    virtual bool                GetSourceLocation(
                                    const tASTLocation& inASTLocation,
                                    SourceLocation&     outSourceLocation) = 0;

                                // Inspect a token at the given source position
    virtual void                Inspect(
                                    const std::string& inSymbol,
                                    size_t             inLine,
                                    size_t             inColumn,
                                    InspectResult&     outResult) = 0;

                                // Return context with program to be inspected
    SpInspectContextI           GetInspectContext();

protected:
                                // Index of shader in program
    size_t                      mShaderIx;

                                // Inspector of program, program inspector is owner of shader inspector
    ProgramInspectorI*          mProgramInspector;
};


// Inline definitions

// Constructor
inline
ShaderInspectorI::ShaderInspectorI(int inShaderIx, ProgramInspectorI* inProgramInspector) :
    mShaderIx         (inShaderIx),
    mProgramInspector (inProgramInspector)
{

}


#endif // __SHADER_INSPECTOR_I_H__
