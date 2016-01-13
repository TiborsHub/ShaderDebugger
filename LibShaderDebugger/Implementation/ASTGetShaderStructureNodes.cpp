/*******************************************************************************\
*                                                                               *
*           Class   : ASTGetShaderStructureNodes                                *
*           Purpose : Computes nodes at which the shader can take different     *
*                     code paths                                                *
*           File    : ASTGetShaderStrutucreNodes.cpp                            *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "ASTGetShaderStructureNodes.h"


// Library headers
#include "ShaderStructureNodes.h"

// ANGLE headers
#include <compiler/translator/IntermNode.h>


// Standard headers
#include <cassert>


namespace
{

class ASTGetShaderStructureNodes : public TIntermTraverser
{
public:
                                // Constructor
                                ASTGetShaderStructureNodes(ShaderStructureNodes& outShaderStructureNodes);

protected:
                                // Visit selection node
    virtual bool                visitSelection(Visit inVisit, TIntermSelection*inSelectionNode) override;

                                // Visit loop node
    virtual bool                visitLoop(Visit inVisit, TIntermLoop* inLoopNode) override;

                                // Visit branch node
    virtual bool                visitBranch(Visit inVisit, TIntermBranch* inBranchNode) override;

                                // Visit aggregate node
    virtual bool                visitAggregate(Visit inVisit, TIntermAggregate* inAggregateNode)  override;

private:
    ShaderStructureNodes&       mShaderStructureNodes;
};


// Utility function to copy a node vector allocated with the pool allocator to vector allocated with the standard allocator
void
AddTVectorNodePath(
    tASTNodeLocation&      ioNodePath,
    TVector<TIntermNode*>& inTVectorNodePath,
    TIntermNode*           inTargetChildNode)
{
    ioNodePath.reserve(inTVectorNodePath.size() + 1);
    ioNodePath.assign(inTVectorNodePath.begin(), inTVectorNodePath.end());
    assert(ioNodePath.back() != inTargetChildNode);
    ioNodePath.push_back(inTargetChildNode);
}


void
AddTVectorNodePath(
    std::vector<tASTNodeLocation>& ioNodePaths,
    TVector<TIntermNode*>&         inTVectorNodePath,
    TIntermNode*                   inTargetChildNode)
{
    ioNodePaths.push_back(tASTNodeLocation());
    AddTVectorNodePath(
        ioNodePaths.back(),
        inTVectorNodePath,
        inTargetChildNode);
}


// Constructor
ASTGetShaderStructureNodes::ASTGetShaderStructureNodes(ShaderStructureNodes& outShaderStructureNodes)
:
    TIntermTraverser      (true, false, false), // pre visit, visit, post visit
    mShaderStructureNodes (outShaderStructureNodes)
{

}


// Visit selection node
// virtual
bool
ASTGetShaderStructureNodes::visitSelection(Visit inVisit, TIntermSelection*inSelectionNode)
{
    (void)inVisit;

    AddTVectorNodePath(mShaderStructureNodes.mSelections, mPath, inSelectionNode);

    return true;
}


// Visit loop node
bool
ASTGetShaderStructureNodes::visitLoop(Visit inVisit, TIntermLoop* inLoopNode)
{
    (void)inVisit;

    AddTVectorNodePath(mShaderStructureNodes.mLoops, mPath, inLoopNode);

    return true;
}


// Visit branch node
bool
ASTGetShaderStructureNodes::visitBranch (Visit inVisit, TIntermBranch* inBranchNode)
{
    (void)inVisit;

    AddTVectorNodePath(mShaderStructureNodes.mBranches, mPath, inBranchNode);

    return true;
}


// Visit aggregate node
bool
ASTGetShaderStructureNodes::visitAggregate(Visit inVisit, TIntermAggregate* inAggregateNode)
{
    (void)inVisit;

    TOperator oper(inAggregateNode->getOp());

    switch (oper)
    {
        case EOpFunction:
            // Declaration of function

            if (inAggregateNode->getName() == "main(")
            {
                assert(mShaderStructureNodes.mMain.empty());

                AddTVectorNodePath(mShaderStructureNodes.mMain, mPath, inAggregateNode);
            }
            else
            {
                AddTVectorNodePath(mShaderStructureNodes.mFunctionDefinitions, mPath, inAggregateNode);
            }
            break;

        case EOpFunctionCall:
            // Call of function
            AddTVectorNodePath(mShaderStructureNodes.mFunctionCalls, mPath, inAggregateNode);
            break;
    }

    return true;
}

}; // namespace


// Return branch nodes from ast
void
GetShaderStructureNodes(TIntermNode* inAST, ShaderStructureNodes& outShaderStructureNodes)
{
    ASTGetShaderStructureNodes branch_nodes_finder(outShaderStructureNodes);
    inAST->traverse(&branch_nodes_finder);
}
