/*******************************************************************************\
*                                                                               *
*           Class   : ProgramInspectorI                                         *
*           Purpose : Interface to hide all details of inspecting a program     *
*           File    : ProgramInspectorI.cpp                                     *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "ProgramInspectorI.h"


// Constructor
ProgramInspectorI::ProgramInspectorI(SpInspectContextI& inInspectContext, size_t inShaderIx) :
    mContext  (inInspectContext),
    mShaderIx (inShaderIx)
{

}
