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
    mInspector->GetNextStatement(mShaderState);

    if (!mShaderState.mCallStack.empty())
    {
        return mInspector->GetSourceLocation(mShaderState.mCallStack.back(), outStepResult.mNextLocation);
    }

    return false;
}


// Reset to initial state
void
Debugger::Reset(DebugResetResult& outResetResult)
{
    mInspector->GetShaderStructure(mShaderState);
    mInspector->GetSourceLocation(mShaderState.mCallStack.back(), outResetResult.mMainLocation);
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
