/*******************************************************************************\
*                                                                               *
*           Class   : ShaderStructureNodes                                      *
*           Purpose : Information about all nodes at which the shader excution  *
*                     path can deviate from sequential execution                *
*           File    : ShaderStructureNodes.h                                    *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __SHADER_STRUCTURE_NODES_H__
#define __SHADER_STRUCTURE_NODES_H__


// Library headers
#include "ASTLocation.h"


// ANGLE headers
#include <compiler/translator/IntermNode.h>


// Standard headers
#include <map>


class ShaderStructureNodes
{
public:
                              // Return function by name
    const tASTLocation&       GetFunction(const std::string& inName) const;

    typedef std::map<std::string, tASTLocation> tFunctionDefinitionsMap;
    tASTLocation              mMain;
    std::vector<tASTLocation> mSelections;
    std::vector<tASTLocation> mBranches;
    std::vector<tASTLocation> mLoops;
    tFunctionDefinitionsMap   mFunctionDefinitions;
    std::vector<tASTLocation> mFunctionCalls;

};


#endif // __SHADER_STRUCTURE_NODES_H__
