/*******************************************************************************\
*                                                                               *
*           Class   : ProgramInspectorWebGL                                     *
*           Purpose : Inspector for WebGL shader programs                       *
*           File    : ProgramInspectorWebGL.h                                   *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __PROGRAM_INSPECTOR_WEBGL_H__
#define __PROGRAM_INSPECTOR_WEBGL_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(ShaderInspectorI);


// Project headers
#include "ProgramInspectorI.h"


// Standard headers
#include <map>


class ProgramInspectorWebGL : public ProgramInspectorI
{
public:
                                // Constructor
                                ProgramInspectorWebGL(SpInspectContextI& inInspectContext, size_t inShaderIx);

                                // Returns ast node of first statement to execute
    virtual TIntermNode*        GetFirstStatement() override;

                                // Inspect a token in a shader at the given source position
    virtual void                Inspect(size_t inSourceIx, InspectResult& outResult) override;

private:
    typedef std::pair<size_t, size_t> tInspectKey;
    typedef std::map<tInspectKey, SpShaderInspectorI> tTokenInspectorMap;
    tTokenInspectorMap          mTokenInspectorMap;
};


#endif // __PROGRAM_INSPECTOR_WEBGL_H__
