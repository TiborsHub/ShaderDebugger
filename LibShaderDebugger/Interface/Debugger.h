/*******************************************************************************\
*                                                                               *
*           Class   : Debugger                                                  *
*           Purpose : Object which allows debugging of the shader               *
*                     by repeatedly executing inspect shaders                   *
*                     It keeps track of both the state of the shader and        *
*                     source location of the symbols to inspect                 *
*           File    : Debugger.h                                                *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
class InspectResult;
class TIntermNode;
class SourceLocation;
CLASS_FORWARD_DECLARATIONS(ProgramInspectorI);


// Library headers
#include "ASTLocation.h"
#include "ShaderStructureState.h"


// Standard headers
#include <vector>


// Result of resetting debug state of shader
struct DebugResetResult
{

};


// Result of advancing debug state of shader to next statement
struct DebugStepResult
{

};


class Debugger
{
public:
                                // Constructor
                                Debugger(SpProgramInspectorI& inInspector);

                                // Advance to next statement
    bool                        Step(DebugStepResult& outStepResult);

                                // Reset to initial state
    void                        Reset(DebugResetResult& outResetResult);

                                // Return location in source of current state
    bool                        GetCurrentLocation(SourceLocation& outSourceLocation);

                                // Inspect symbol at location
    bool                        Inspect(size_t inShaderIx, size_t inSourceIx, InspectResult& outResult);

private:
    SpProgramInspectorI         mInspector;
    tASTLocation                mCurrentStatement;
    ShaderStructureState        mShaderState;
};


#endif // __DEBUGGER_H__
