/*******************************************************************************\
*                                                                               *
*           Class   : Debugger                                                  *
*           Purpose : Object which allows debugging of the shader               *
*                     by repeatedly executing inspect shaders                   *
*                     It keeps track of both the state of the shader and        *
*                     source location of the symbols to inspect                 *
*           File    : Debugger.h                                                    *
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
CLASS_FORWARD_DECLARATIONS(ProgramInspectorI);


class Debugger
{
public:
                                // Constructor
                                Debugger();

                                // Moves to next statement
    bool                        Next();

                                // Reset to initial state
    void                        Reset();

                                // Return location in source of current state
    bool                        GetCurrentLocation();

                                // Inspect symbol at location
    bool                        Inspect(size_t inShaderIx, size_t inSourceIx, InspectResult& outResult);

private:
    SpProgramInspectorI         mInspector;
    TIntermNode*                mCurrentStatement;
};


#endif // __DEBUGGER_H__
