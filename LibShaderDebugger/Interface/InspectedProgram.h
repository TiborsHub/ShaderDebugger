/*******************************************************************************\
*																				*
*			Class    : InspectedProgram					                        *
*			Purpose  : Keeps track of corresponding inspect context and         *
*                      inspector objects                                        *
*			File     : InspectedProgram.h							            *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __INSPECTED_PROGRAM_H__
#define __INSPECTED_PROGRAM_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(InspectContextI)
CLASS_FORWARD_DECLARATIONS(ProgramInspectorI)


// Standard headers
#include <string>


class InspectedProgram
{
public:
                                // Constructor
                                InspectedProgram(
                                    const std::string&   inId,
                                    SpInspectContextI&   inInspectContext,
                                    SpProgramInspectorI& inInspector);

                                // Return unique id of inspect context - program combination
    const std::string&          GetId() const;

                                // Return inspect context
    InspectContextI*            GetInspectContext();

                                // Return inspector
    ProgramInspectorI*          GetInspector();

private:
    std::string                 mId;
    SpInspectContextI           mInspectContext;
    SpProgramInspectorI         mInspector;
};


// Inline definitions

// Constructor
inline
InspectedProgram::InspectedProgram(
    const std::string&   inId,
    SpInspectContextI&   inInspectContext,
    SpProgramInspectorI& inInspector)
:
    mId             (inId),
    mInspectContext (inInspectContext),
    mInspector      (inInspector)
{
}


// Return unique id of inspect context - program combination
inline
const std::string&
InspectedProgram::GetId() const
{
    return mId;
}


// Return inspect context
inline
InspectContextI*
InspectedProgram::GetInspectContext()
{
    return mInspectContext.get();
}


// Return inspector
inline
ProgramInspectorI*
InspectedProgram::GetInspector()
{
    return mInspector.get();
}


#endif // __INSPECTED_PROGRAM_H__
