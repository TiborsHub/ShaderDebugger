/*******************************************************************************\
*																				*
*			Class   : ShaderInspectorI                                          *
*			Purpose : Interface to inspector shader                             *
*			File    : ShaderInspectorI.h							                *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __SHADER_INSPECTOR_I_H__
#define __SHADER_INSPECTOR_I_H__


#include "SmartPointerDeclarations.h"


// Forward declaration
class InspectResult;
CLASS_FORWARD_DECLARATIONS(ProgramInspectorI);
CLASS_FORWARD_DECLARATIONS(InspectContextI);


// Standard headers
#include <string>


class ShaderInspectorI
{
public:
                                // Constructor
                                ShaderInspectorI(int inShaderIx, SpProgramInspectorI inProgramInspector);

                                // Destructor
    virtual                     ~ShaderInspectorI() {};

                                // Initialize the inspector
    virtual bool                Initialize() = 0;

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

                                // Inspector of program
    SpProgramInspectorI         mProgramInspector;
};


// Inline definitions

// Constructor
inline
ShaderInspectorI::ShaderInspectorI(int inShaderIx, SpProgramInspectorI inProgramInspector) :
    mShaderIx         (inShaderIx),
    mProgramInspector (inProgramInspector)
{

}


#endif // __SHADER_INSPECTOR_I_H__
