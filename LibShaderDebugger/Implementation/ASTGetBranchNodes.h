/*******************************************************************************\
*                                                                               *
*           Class   : ASTGetBranchNodes                                         *
*           Purpose : Computes list of nodes at which the shader can take       *
*                     different code paths                                      *
*           File    : ASTGetBranchNodes.h                                       *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __AST_GET_BRANCH_NODES_H__
#define __AST_GET_BRANCH_NODES_H__


// Forward declarations
class TIntermNode;


// Standard headers
#include <vector>


struct BranchNodes
{
                                // Constructor
                                BranchNodes();

    TIntermNode*                mMain;
    std::vector<TIntermNode*>   mSelections;
    std::vector<TIntermNode*>   mBranches;
    std::vector<TIntermNode*>   mLoops;
    std::vector<TIntermNode*>   mFunctionDefinitions;
    std::vector<TIntermNode*>   mFunctionCalls;
};


// Inline defines

// Constructor
inline
BranchNodes::BranchNodes() :
    mMain(nullptr)
{

}


// Return branch nodes from ast
void
GetBranchNodes(TIntermNode* inAST, BranchNodes& outBranchNodes);


#endif // __AST_GET_BRANCH_NODES_H__
