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
#include "ASTNodeLocation.h"
#include "ASTNodeLocationConversion.h"


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
GetNodeLocation(
    TVector<TIntermNode*>& inTVectorNodePath,
    TIntermNode*           inTargetChildNode,
    tASTLocation&          outNodePath)
{
    tASTNodeLocation node_location;
    node_location.reserve(inTVectorNodePath.size() + 1);
    node_location.assign(inTVectorNodePath.begin(), inTVectorNodePath.end());
    node_location.push_back(inTargetChildNode);

    GetNodeIndexPath(node_location, outNodePath);
}


void
AddNodeLocation(
    TVector<TIntermNode*>&     inTVectorNodePath,
    TIntermNode*               inTargetChildNode,
    std::vector<tASTLocation>& ioNodePaths)
{
    ioNodePaths.push_back(tASTLocation());
    GetNodeLocation(inTVectorNodePath, inTargetChildNode, ioNodePaths.back());
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

    AddNodeLocation(mPath, inSelectionNode, mShaderStructureNodes.mSelections);

    return true;
}


// Visit loop node
bool
ASTGetShaderStructureNodes::visitLoop(Visit inVisit, TIntermLoop* inLoopNode)
{
    (void)inVisit;

    AddNodeLocation(mPath, inLoopNode, mShaderStructureNodes.mLoops);

    return true;
}


// Visit branch node
bool
ASTGetShaderStructureNodes::visitBranch (Visit inVisit, TIntermBranch* inBranchNode)
{
    (void)inVisit;

    AddNodeLocation(mPath, inBranchNode, mShaderStructureNodes.mBranches);

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

                GetNodeLocation(mPath, inAggregateNode, mShaderStructureNodes.mMain);
            }
            else
            {
                std::string function_name(inAggregateNode->getName().c_str());
                tASTLocation function_loc;
                GetNodeLocation(mPath, inAggregateNode, function_loc);

                auto result(mShaderStructureNodes.mFunctionDefinitions.insert(std::make_pair(function_name, function_loc)));
            }
            break;

        case EOpFunctionCall:
            // Call of function
            AddNodeLocation(mPath, inAggregateNode, mShaderStructureNodes.mFunctionCalls);
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
