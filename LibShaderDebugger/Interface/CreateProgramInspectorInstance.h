/*******************************************************************************\
*																				*
*			Function : CreateProgramInspectorInstance					        *
*			Purpose  : Factory function to create concrete instance of          *
*                      program inspector                                        *
*			File     : CreateProgramInspectorInstance.h						    *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __CREATE_PROGRAM_INSPECTOR_INSTANCE_H__
#define __CREATE_PROGRAM_INSPECTOR_INSTANCE_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(ProgramInspectorI);
CLASS_FORWARD_DECLARATIONS(InspectContextI);


// Standard headers
#include <string>


SpProgramInspectorI
CreateProgramInspectorInstance(SpInspectContextI& inInspectContext);


#endif // __CREATE_PROGRAM_INSPECTOR_INSTANCE_H__
