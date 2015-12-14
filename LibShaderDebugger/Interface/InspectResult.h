/*******************************************************************************\
*																				*
*			Class   : InspectResult	                                            *
*			Purpose : Result of inspection request                              *
*			File    : InspectResult.h							                *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __INSPECT_RESULT_H__
#define __INSPECT_RESULT_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(PixelData2dF4);


// Standard headers
#include <string>


// Inspect result codes
enum InspectResultCode
{
    // Inspect result is not set
    INSPECT_RESULT_UNKNOWN,

    // Index of character position in source is invalid
    INSPECT_RESULT_INVALID_SOURCE_POS,

    // Initialization of shader inspector failed
    INSPECT_RESULT_SHADER_INSPECTOR_INITIALIZATION_FAILED,

    // No inspect information available
    INSPECT_RESULT_TOKEN_NO_INFO,

    // Target symbol not found in shader source
    INSPECT_RESULT_SYMBOL_NOT_FOUND,

    // Transformations on AST to retrieve target symbol are not yet implemented
    INSPECT_RESULT_NOT_YET_IMPLEMENTED,
};


class InspectResult
{
public:
                                // Constructor
                                InspectResult();

    InspectResultCode           mResultCode;
    std::string                 mInfoText;
    SpConstPixelData2dF4        mInspectData;
};


#endif // __INSPECT_RESULT_H__
