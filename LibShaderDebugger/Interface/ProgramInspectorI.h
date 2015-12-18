/*******************************************************************************\
*																				*
*			Class   : ProgramInspectorI					                        *
*			Purpose : Interface to hide all details of inspecting a program     *
*			File    : ProgramInspectorI.h							            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __PROGRAM_INSPECTOR_I_H__
#define __PROGRAM_INSPECTOR_I_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(InspectContextI);
class InspectResult;


// Standard headers
#include <string>


class ProgramInspectorI
{
public:
                                // Constructor
                                ProgramInspectorI(SpInspectContextI& inInspectContext);

                                // Destructor
    virtual                     ~ProgramInspectorI() {};

                                // Returns context
    SpInspectContextI           GetContext();

                                // Inspect a token in a shader at the given source position
    virtual void                Inspect(size_t inShaderIx, size_t inSourceIx, InspectResult& outResult) = 0;

protected:
    SpInspectContextI           mContext;
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
