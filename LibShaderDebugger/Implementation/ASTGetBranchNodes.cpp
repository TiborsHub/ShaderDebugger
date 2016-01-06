/*******************************************************************************\
*                                                                               *
*           Class   : ASTGetBranchNodes                                         *
*           Purpose : Computes list of nodes at which the shader can take       *
*                     different code paths                                      *
*           File    : ASTGetBranchNodes.cpp                                     *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "ASTGetBranchNodes.h"


// ANGLE headers
#include <compiler/translator/IntermNode.h>


namespace
{

class ASTGetBranchNodes : public TIntermTraverser
{
public:
                                // Constructor
                                ASTGetBranchNodes(BranchNodes& outBranchNodes);

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
    BranchNodes&                mBranchNodes;
};


// Constructor
ASTGetBranchNodes::ASTGetBranchNodes(BranchNodes& outBranchNodes)
:
    TIntermTraverser (true, false, false), // pre visit, visit, post visit
    mBranchNodes     (outBranchNodes)
{

}


// Visit selection node
// virtual
bool
ASTGetBranchNodes::visitSelection(Visit inVisit, TIntermSelection*inSelectionNode)
{
    (void)inVisit;

    mBranchNodes.mSelections.push_back(inSelectionNode);

    return true;
}


// Visit loop node
bool
ASTGetBranchNodes::visitLoop(Visit inVisit, TIntermLoop* inLoopNode)
{
    (void)inVisit;

    mBranchNodes.mLoops.push_back(inLoopNode);

    return true;
}


// Visit branch node
bool
ASTGetBranchNodes::visitBranch (Visit inVisit, TIntermBranch* inBranchNode)
{
    (void)inVisit;

    mBranchNodes.mBranches.push_back(inBranchNode);

    return true;
}


// Visit aggregate node
bool
ASTGetBranchNodes::visitAggregate(Visit inVisit, TIntermAggregate* inAggregateNode)
{
    (void)inVisit;

    TOperator oper(inAggregateNode->getOp());

    switch (oper)
    {
        case EOpFunction:
            // Declaration of function

            if (inAggregateNode->getName() == "main(")
            {
                assert(mBranchNodes.mMain == nullptr);

                mBranchNodes.mMain = inAggregateNode;
            }
            else
            {
                mBranchNodes.mFunctionDefinitions.push_back(inAggregateNode);
            }
            break;

        case EOpFunctionCall:
            // Call of function
            mBranchNodes.mFunctionCalls.push_back(inAggregateNode);
            break;
    }

    return true;
}

}; // namespace


// Return branch nodes from ast
void
GetBranchNodes(TIntermNode* inAST, BranchNodes& outBranchNodes)
{
    ASTGetBranchNodes branch_nodes_finder(outBranchNodes);
    inAST->traverse(&branch_nodes_finder);
}
