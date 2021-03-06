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

                                // Returns information about the structure of the shader
    virtual void                GetShaderStructure(ShaderStructureState& outShaderStructure) override;

                                // Return next statement
    virtual bool                GetNextStatement(ShaderStructureState& ioCurrentState);

                                // Return location in source code for an ast node
    virtual bool                GetSourceLocation(
                                    const tASTLocation& inASTLocation,
                                    SourceLocation&     outSourceLocation) override;

                                // Inspect a token in a shader at the given source position
    virtual void                Inspect(size_t inSourceIx, InspectResult& outResult) override;

private:
    typedef std::pair<size_t, size_t> tInspectKey;
    typedef std::map<tInspectKey, SpShaderInspectorI> tTokenInspectorMap;
    tTokenInspectorMap          mTokenInspectorMap;

    SpShaderInspectorI          mShaderInspector;
};


#endif // __PROGRAM_INSPECTOR_WEBGL_H__
