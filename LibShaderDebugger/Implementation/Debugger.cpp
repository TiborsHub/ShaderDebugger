/*******************************************************************************\
*                                                                               *
*           Class   : Debugger                                                  *
*           Purpose : Object which allows debugging of the shader               *
*                     by repeatedly executing inspect shaders                   *
*                     It keeps track of both the state of the shader and        *
*                     source location of the symbols to inspect                 *
*           File    : Debugger.cpp                                              *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "Debugger.h"


// Library headers
#include "ProgramInspectorI.h"


// Constructor
Debugger::Debugger(SpProgramInspectorI& inInspector) :
    mInspector(inInspector)
{

}


// Advance to next statement
bool
Debugger::Step(DebugStepResult& outStepResult)
{
    mInspector->GetNextStatement(
        mCurrentStatement,
        mShaderState,
        mCurrentStatement);

    return false;
}


// Reset to initial state
void
Debugger::Reset(DebugResetResult& outResetResult)
{
//    mCurrentStatement = mInspector->GetFirstStatement();
}


// Return location in source of current state
bool
Debugger::GetCurrentLocation(SourceLocation& outSourceLocation)
{
    return false;
}


// Inspect symbol at location
bool
Debugger::Inspect(size_t inShaderIx, size_t inSourceIx, InspectResult& outResult)
{
    return false;
}