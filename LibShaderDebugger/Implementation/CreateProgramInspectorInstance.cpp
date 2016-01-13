/*******************************************************************************\
*                                                                               *
*           Function : CreateProgramInspectorInstance                           *
*           Purpose  : Factory function to create concrete instance of          *
*                      program inspector                                        *
*           File     : CreateProgramInspectorInstance.cpp                       *
*           Author   : Tibor den Ouden                                          *
*                                                                               *
\*******************************************************************************/


// Translation unit headers
#include "CreateProgramInspectorInstance.h"


// Project headers
#include "ProgramInspectorWebGL.h"


SpProgramInspectorI
CreateProgramInspectorInstance(SpInspectContextI& inInspectContext, size_t inShaderIx)
{
    // Create inspector based on type of context
    return std::make_shared<ProgramInspectorWebGL>(inInspectContext, inShaderIx);
}
