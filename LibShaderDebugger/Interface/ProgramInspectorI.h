/*******************************************************************************\
*                                                                               *
*           Class   : ProgramInspectorI                                         *
*           Purpose : Interface to hide all details of inspecting a program     *
*           File    : ProgramInspectorI.h                                       *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __PROGRAM_INSPECTOR_I_H__
#define __PROGRAM_INSPECTOR_I_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(InspectContextI);
class InspectResult;
class TIntermNode;
class SourceLocation;
class ShaderStructureState;


// Library headers
#include "ASTLocation.h"


// Standard headers
#include <string>


class ProgramInspectorI
{
public:
                                // Constructor
                                ProgramInspectorI(SpInspectContextI& inInspectContext, size_t inShaderIx);

                                // Destructor
    virtual                     ~ProgramInspectorI() {};

                                // Returns context
    SpInspectContextI           GetContext();

                                // Returns information about the structure of the shader
    virtual void                GetShaderStructure(ShaderStructureState& outShaderStructure) = 0;

                                // Return next statement
    virtual void                GetNextStatement(
                                    const tASTLocation&         inCurrLocation,
                                    const ShaderStructureState& inCurrentState,
                                    const tASTLocation&         outNextLocation) = 0;

                                // Return location in source code for an ast node
    virtual bool                GetSourceLocation(
                                    const tASTLocation& inASTLocation,
                                    SourceLocation&     outSourceLocation) = 0;

                                // Inspect a token in a shader at the given source position
    virtual void                Inspect(size_t inSourceIx, InspectResult& outResult) = 0;

protected:
    SpInspectContextI           mContext;
    size_t                      mShaderIx;
};


// Inline definitions

// Returns context
inline
SpInspectContextI
ProgramInspectorI::GetContext()
{
    return mContext;
}


#endif // __PROGRAM_INSPECTOR_I_H__
