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
#include "ASTNodeLocation.h"


class ShaderStructureNodes
{
public:
    tASTNodeLocation              mMain;
    std::vector<tASTNodeLocation> mSelections;
    std::vector<tASTNodeLocation> mBranches;
    std::vector<tASTNodeLocation> mLoops;
    std::vector<tASTNodeLocation> mFunctionDefinitions;
    std::vector<tASTNodeLocation> mFunctionCalls;
};


#endif // __SHADER_STRUCTURE_NODES_H__
