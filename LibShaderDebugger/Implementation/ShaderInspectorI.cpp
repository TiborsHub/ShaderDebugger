/*******************************************************************************\
*                                                                               *
*           Class   : ShaderInspectorI                                          *
*           Purpose : Interface to inspector shader                             *
*           File    : ShaderInspectorI.cpp                                      *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "ShaderInspectorI.h"


// Library headers
#include "ProgramInspectorI.h"


// Return context with program to be inspected
SpInspectContextI
ShaderInspectorI::GetInspectContext()
{
    return mProgramInspector->GetContext();
}
