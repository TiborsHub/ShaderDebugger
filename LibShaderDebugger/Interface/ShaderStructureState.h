/*******************************************************************************\
*                                                                               *
*           Class   : ShaderStructureState                                      *
*           Purpose : Contains all locations of nodes where the execution path  *
*                     can branch and the state of the branch variable           *
*           File    : ShaderStructureState.h                                    *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __SHADER_STRUCTURE_STATE_H__
#define __SHADER_STRUCTURE_STATE_H__


// Library headers
#include "ASTLocation.h"


// Standard headers
#include <vector>


class ShaderStructureState
{
public:
    tASTLocation                mMain;
    std::vector<tASTLocation>   mSelections;
    std::vector<tASTLocation>   mBranches;
    std::vector<tASTLocation>   mLoops;
    std::vector<tASTLocation>   mFunctionDefinitions;
    std::vector<tASTLocation>   mFunctionCalls;
};


#endif // __SHADER_STRUCTURE_STATE_H__
