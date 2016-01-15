/*******************************************************************************\
*                                                                               *
*           Utility : ASTQueries                                                *
*           Purpose : Queries to compute certain properties of an AST           *
*           File    : ASTQueries.cpp                                            *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "ASTQueries.h"


// ANGLE headers
#include <compiler/translator/IntermNode.h>


// Standard headers
#include <assert.h>


namespace
{

// Utility class to compute node type
class TIntermNodeTypeEvaluator : public TIntermTraverser
{
public:
                                // Constructor
                                TIntermNodeTypeEvaluator() :
                                    TIntermTraverser (false, true, false),
                                    mNodeType        (AST_NODE_TYPE_UNKNOWN)
                                {}

                                // Traverse node
    virtual void                traverseSymbol        (TIntermSymbol*       ) override { mNodeType = AST_NODE_TYPE_SYMBOL;         }
    virtual void                traverseRaw           (TIntermRaw*          ) override { mNodeType = AST_NODE_TYPE_RAW;            }
    virtual void                traverseConstantUnion (TIntermConstantUnion*) override { mNodeType = AST_NODE_TYPE_CONSTANT_UNION; }
    virtual void                traverseBinary        (TIntermBinary*       ) override { mNodeType = AST_NODE_TYPE_BINARY;         }
    virtual void                traverseUnary         (TIntermUnary*        ) override { mNodeType = AST_NODE_TYPE_UNARY;          }
    virtual void                traverseSelection     (TIntermSelection*    ) override { mNodeType = AST_NODE_TYPE_SELECTION;      }
    virtual void                traverseSwitch        (TIntermSwitch*       ) override { mNodeType = AST_NODE_TYPE_SWITCH;         }
    virtual void                traverseCase          (TIntermCase*         ) override { mNodeType = AST_NODE_TYPE_CASE;           }
    virtual void                traverseAggregate     (TIntermAggregate*    ) override { mNodeType = AST_NODE_TYPE_AGGREGATE;      }
    virtual void                traverseLoop          (TIntermLoop*         ) override { mNodeType = AST_NODE_TYPE_LOOP;           }
    virtual void                traverseBranch        (TIntermBranch*       ) override { mNodeType = AST_NODE_TYPE_BRANCH;         }

                                // Returns type of node
    ASTNodeType                 GetNodeType() const   { return mNodeType; }

private:
    ASTNodeType                 mNodeType;
};


// Utility class to compute child node index
class FindNextChildNodeAtSameDepth : public TIntermTraverser
{
public:
                                // Constructor
                                FindNextChildNodeAtSameDepth(TIntermNode* inTargetChildNode);

                                // Return next child node
    TIntermNode*                GetNextChildNode() const { return mNextChildNode; }

                                // Compute index in list of direct children according to traverse order
    static TIntermNode*         sGetNextChildNode(TIntermNode* inParentNode, TIntermNode* inChildNode);

private:
                                // Visit of terminal nodes
    virtual void                visitSymbol        (TIntermSymbol*        inNode) override { visitTerminalNode(inNode); }
    virtual void                visitRaw           (TIntermRaw*           inNode) override { visitTerminalNode(inNode); }
    virtual void                visitConstantUnion (TIntermConstantUnion* inNode) override { visitTerminalNode(inNode); }

                                // Visit of non-terminal nodes
    virtual bool                visitBinary    (Visit inVisit, TIntermBinary*    inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitUnary     (Visit inVisit, TIntermUnary*     inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitSelection (Visit inVisit, TIntermSelection* inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitSwitch    (Visit inVisit, TIntermSwitch*    inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitCase      (Visit inVisit, TIntermCase*      inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitAggregate (Visit inVisit, TIntermAggregate* inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitLoop      (Visit inVisit, TIntermLoop*      inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitBranch    (Visit inVisit, TIntermBranch*    inNode) override { return visitNonTerminalNode(inVisit, inNode); }

                                // Common implementation for terminal nodes
    void                        visitTerminalNode(TIntermNode* inNode);

                                // Common implementation of non terminal nodes
    bool                        visitNonTerminalNode(Visit inVisit, TIntermNode* inNode);

    TIntermNode*                mTargetChildNode;
    bool                        mTargetFound;
    TIntermNode*                mNextChildNode;
};


// Constructor
FindNextChildNodeAtSameDepth::FindNextChildNodeAtSameDepth(TIntermNode* inTargetChildNode) :
    TIntermTraverser (true, false, false), // pre visit, visit, post visit
    mTargetChildNode (inTargetChildNode),
    mTargetFound     (false),
    mNextChildNode   (nullptr)
{

}


// Common implementation for terminal nodes
void
FindNextChildNodeAtSameDepth::visitTerminalNode(TIntermNode* inNode)
{
    if (mDepth == 1)
    {
        if (mTargetFound)
        {
            mNextChildNode = inNode;
        }
        else
        {
            mTargetFound = (inNode == mTargetChildNode);
        }
    }
}


// Common implementation of non terminal nodes
bool
FindNextChildNodeAtSameDepth::visitNonTerminalNode(Visit inVisit, TIntermNode* inNode)
{
    if (mDepth == 1)
    {
        if (mTargetFound)
        {
            mNextChildNode = inNode;
        }
        else
        {
            mTargetFound = (inNode == mTargetChildNode);
        }
    }

    return (mNextChildNode == nullptr);
}


// Compute index in list of direct children according to traverse order
//static
TIntermNode*
FindNextChildNodeAtSameDepth::sGetNextChildNode(TIntermNode* inParentNode, TIntermNode* inChildNode)
{
    FindNextChildNodeAtSameDepth find_next_child_node(inChildNode);
    inParentNode->traverse(&find_next_child_node);

    return find_next_child_node.GetNextChildNode();
}


// Utility class to compute child node index
class FindNextChildNodeDepthFirst : public TIntermTraverser
{
public:
                                // Constructor
                                FindNextChildNodeDepthFirst(TIntermNode* inParentNode,TIntermNode* inTargetChildNode);

                                // Return next child node
    TIntermNode*                GetNextChildNode() const { return mNextChildNode; }

                                // Find next child node
                                // the child node to pass can be nullptr
    static TIntermNode*         sGetNextChildNode(TIntermNode* inParentNode, TIntermNode* inChildNode);

private:
                                // Visit of terminal nodes
    virtual void                visitSymbol        (TIntermSymbol*        inNode) override { VisitNode(inNode); }
    virtual void                visitRaw           (TIntermRaw*           inNode) override { VisitNode(inNode); }
    virtual void                visitConstantUnion (TIntermConstantUnion* inNode) override { VisitNode(inNode); }

                                // Visit of non-terminal nodes
    virtual bool                visitBinary    (Visit, TIntermBinary*    inNode) override { return VisitNode(inNode); }
    virtual bool                visitUnary     (Visit, TIntermUnary*     inNode) override { return VisitNode(inNode); }
    virtual bool                visitSelection (Visit, TIntermSelection* inNode) override { return VisitNode(inNode); }
    virtual bool                visitSwitch    (Visit, TIntermSwitch*    inNode) override { return VisitNode(inNode); }
    virtual bool                visitCase      (Visit, TIntermCase*      inNode) override { return VisitNode(inNode); }
    virtual bool                visitAggregate (Visit, TIntermAggregate* inNode) override { return VisitNode(inNode); }
    virtual bool                visitLoop      (Visit, TIntermLoop*      inNode) override { return VisitNode(inNode); }
    virtual bool                visitBranch    (Visit, TIntermBranch*    inNode) override { return VisitNode(inNode); }

                                // Common implementation for terminal nodes
    bool                        VisitNode(TIntermNode* inNode);

    TIntermNode*                mParentNode;
    TIntermNode*                mTargetChildNode;
    TIntermNode*                mNextChildNode;
};


// Constructor
FindNextChildNodeDepthFirst::FindNextChildNodeDepthFirst(
    TIntermNode* inParentNode,
    TIntermNode* inTargetChildNode)
:
    TIntermTraverser (true, false, false), // pre visit, visit, post visit
    mParentNode      (inParentNode),
    mTargetChildNode (inTargetChildNode),
    mNextChildNode   (nullptr)
{
    assert(IsDirectChildNode(inParentNode, inTargetChildNode));
}


// Common implementation for all nodes
bool
FindNextChildNodeDepthFirst::VisitNode(TIntermNode* inNode)
{
    if (inNode != mParentNode)
    {
        if (mTargetChildNode == nullptr)
        {
            // Target child node has been passed or was not set
            if (mNextChildNode == nullptr)
            {
                mNextChildNode = inNode;
            }

            // To stop traversing
            return false;
        }
        else
        {
            if (mTargetChildNode == inNode)
            {
                // Remove target child node, next node in traverse is the one to return
                mTargetChildNode = nullptr;
            }
        }
    }

    // Continue traversing
    return true;
}


// Find next child node
// the child node to pass can be nullptr
// static
TIntermNode*
FindNextChildNodeDepthFirst::sGetNextChildNode(TIntermNode* inParentNode, TIntermNode* inChildNode)
{
    FindNextChildNodeDepthFirst next_node(inParentNode, inChildNode);
    inParentNode->traverse(&next_node);
    return next_node.GetNextChildNode();
}


// Utility class to compute child node index
class DirectChildTester : public TIntermTraverser
{
public:
                                // Constructor
                                DirectChildTester(TIntermNode* inTargetChildNode);

                                // Returns if child is direct descendent of parent
    bool                        IsDirectChildNode() const { return (mTargetChildNode == nullptr); }

                                // Returns if child is direct descendent of parent
    static bool                 sIsDirectChildNode(TIntermNode* inParentNode, TIntermNode* inChildNode);

private:
                                // Visit of terminal nodes
    virtual void                visitSymbol        (TIntermSymbol*        inNode) override { VisitNode(inNode); }
    virtual void                visitRaw           (TIntermRaw*           inNode) override { VisitNode(inNode); }
    virtual void                visitConstantUnion (TIntermConstantUnion* inNode) override { VisitNode(inNode); }

                                // Visit of non-terminal nodes
    virtual bool                visitBinary    (Visit, TIntermBinary*    inNode) override { return VisitNode(inNode); }
    virtual bool                visitUnary     (Visit, TIntermUnary*     inNode) override { return VisitNode(inNode); }
    virtual bool                visitSelection (Visit, TIntermSelection* inNode) override { return VisitNode(inNode); }
    virtual bool                visitSwitch    (Visit, TIntermSwitch*    inNode) override { return VisitNode(inNode); }
    virtual bool                visitCase      (Visit, TIntermCase*      inNode) override { return VisitNode(inNode); }
    virtual bool                visitAggregate (Visit, TIntermAggregate* inNode) override { return VisitNode(inNode); }
    virtual bool                visitLoop      (Visit, TIntermLoop*      inNode) override { return VisitNode(inNode); }
    virtual bool                visitBranch    (Visit, TIntermBranch*    inNode) override { return VisitNode(inNode); }

                                // Common implementation for terminal nodes
    bool                        VisitNode(TIntermNode* inNode);

    TIntermNode*                mTargetChildNode;
};


// Constructor
DirectChildTester::DirectChildTester(TIntermNode* inTargetChildNode) :
    TIntermTraverser (true, false, false), // pre visit, visit, post visit
    mTargetChildNode (inTargetChildNode)
{

}


// Common implementation for all nodes
bool
DirectChildTester::VisitNode(TIntermNode* inNode)
{
    if (inNode == mTargetChildNode && mDepth == 1)
    {
        // Child node found
        mTargetChildNode = nullptr;
    }

    // Continue traversing if target node not found and tree maximum one level deep
    return (mTargetChildNode != nullptr && mDepth <= 1);
}


// Returns if child is direct descendent of parent
// static
bool
DirectChildTester::sIsDirectChildNode(TIntermNode* inParentNode, TIntermNode* inChildNode)
{
    DirectChildTester is_direct_child(inChildNode);
    inParentNode->traverse(&is_direct_child);
    return is_direct_child.IsDirectChildNode();
}

}; // namespace


// Return type of node
ASTNodeType
GetNodeType(TIntermNode* inNode)
{
    TIntermNodeTypeEvaluator node_type_eval;
    inNode->traverse(&node_type_eval);
    return node_type_eval.GetNodeType();
}


// Returns operator if node is a binary node
TOperator
GetBinaryOperator(size_t inNodeIx, const tASTNodeLocation& inSymbolNodePath)
{
    assert(inNodeIx < inSymbolNodePath.size());

    TIntermBinary* op_node(inSymbolNodePath[inNodeIx]->getAsBinaryNode());
    return (op_node != nullptr) ? op_node->getOp() : EOpNull;
}


// Returns true if target node is part of a declaration
bool
IsInDeclaration(const tASTNodeLocation& inSymbolNodePath)
{
    TIntermNode* parent(inSymbolNodePath[inSymbolNodePath.size() - 2]);
    TIntermAggregate* op_node(parent->getAsAggregate());

    return (op_node != nullptr) ? (op_node->getOp() == EOpDeclaration) : false;
}


// Return true if target node is part of an assignment
bool
IsInAssignment(const tASTNodeLocation& inSymbolNodePath)
{
    int parent_ix(inSymbolNodePath.size() - 2);
    TOperator parent_node_op(GetBinaryOperator(parent_ix, inSymbolNodePath));
    if (parent_node_op == EOpVectorSwizzle || parent_node_op == EOpIndexDirect)
    {
        // One node up in the tree should be the operation on the target
        assert(parent_ix > 0);
        --parent_ix;
        parent_node_op = GetBinaryOperator(parent_ix, inSymbolNodePath);
    }

    switch (parent_node_op)
    {
        case EOpAssign:
        case EOpInitialize:
        case EOpAddAssign:
        case EOpSubAssign:
        case EOpMulAssign:
        case EOpDivAssign:
        case EOpIModAssign:
            return true;

        default:
            return false;
    }
}


// Find sequence statement which the direct parent of the target (last) node
int
FindDirectParentSequence(const tASTNodeLocation& inSymbolNodePath)
{
    for (int n_ix(inSymbolNodePath.size() - 1); n_ix > -1; --n_ix)
    {
        TIntermAggregate* agg_node(inSymbolNodePath[n_ix]->getAsAggregate());
        if (agg_node != nullptr && agg_node->getOp() == EOpSequence)
        {
            return n_ix;
        }
    }

    // Every shader program is a sequence at the highest level
    // So should never end up here
    assert(false);
    return -1;
}


// Find target node in sequence
int
FindNodeinSequence(TIntermNode* inTargetNode, TIntermAggregate* inSequenceNode)
{
    assert(inSequenceNode->getOp() == EOpSequence);

    TIntermSequence* seq(inSequenceNode->getSequence());
    int s_ix(0);
    int size(seq->size());
    for (; s_ix < size && seq->at(s_ix) != inTargetNode; ++s_ix)
    {}

    return (s_ix == size) ? -1 : s_ix;
}


// Return count of scalars from symbol
int
GetScalarCountFromSymbolNode(TIntermSymbol* inSymbolNode)
{
    if (inSymbolNode->isScalar())
    {
        return 1;
    }
    else if (inSymbolNode->isVector())
    {
        return inSymbolNode->getNominalSize();
    }
    else if (inSymbolNode->isMatrix())
    {
        return inSymbolNode->getCols() * inSymbolNode->getRows();
    }

    assert(false);
    return 0;
}


// Return count of scalars in expression
int
GetScalarCountFromExpression(TIntermNode* inTargetNode)
{
    TIntermSymbol* symbol(inTargetNode->getAsSymbolNode());
    if (symbol != nullptr)
    {
        return GetScalarCountFromSymbolNode(symbol);
    }
    else
    {
        TIntermBinary* selector(inTargetNode->getAsBinaryNode());
        if (selector != nullptr)
        {
            TOperator oper(selector->getOp());
            if (oper == EOpVectorSwizzle)
            {
                // Right term should be sequence
                // Length of sequence is count of selected members
                TIntermAggregate* seq_node(selector->getRight()->getAsAggregate());
                assert(seq_node->getOp() == EOpSequence);
                return seq_node->getSequence()->size();
            }
            else if (oper == EOpIndexDirect)
            {
                // Indexed member
                return 1;
            }
            else if (oper == EOpInitialize)
            {
                return GetScalarCountFromSymbolNode(selector->getLeft()->getAsSymbolNode());
            }
        }
    }

    assert(false);
    return 0;
}


// Returns top node which makes up the value targeted by the symbol node (front node)
TIntermNode*
FindValueExpressionNode(const tASTNodeLocation& inSymbolNodePath)
{
    assert(inSymbolNodePath.back()->getAsSymbolNode() != nullptr);
    assert(inSymbolNodePath.size() > 1);

    // Test if parent of symbol node is member selection
    size_t top_value_ix(inSymbolNodePath.size() - 2);

    TOperator oper(GetBinaryOperator(top_value_ix, inSymbolNodePath));
    if (oper == EOpVectorSwizzle || oper == EOpIndexDirect)
    {
        return inSymbolNodePath[top_value_ix];
    }
    else
    {
        return inSymbolNodePath.back();
    }
}


// Return next child node in AST from current node
void
GetNextChildNodeAtSameDepth(
    const tASTNodeLocation& inCurrNodePath,
    tASTNodeLocation&       outNextNodePath)
{
    // Test if node path contains at least a parent and a child node
    if (inCurrNodePath.size() >= 2)
    {
        tASTNodeLocation::const_reverse_iterator parent_it(inCurrNodePath.rbegin() + 1);
        TIntermNode* next_child_node;
        do
        {
            next_child_node = FindNextChildNodeAtSameDepth::sGetNextChildNode(*parent_it, *(parent_it - 1));
            ++parent_it;
        }
        while (next_child_node == nullptr && parent_it < inCurrNodePath.rend());

        if (next_child_node != nullptr)
        {
            outNextNodePath.reserve(parent_it.base() - inCurrNodePath.begin() + 2);
            // Copy path to parent (including parent)
            outNextNodePath.assign(inCurrNodePath.begin(), parent_it.base() + 1);

            outNextNodePath.push_back(next_child_node);
        }
    }
}


// Return next child node in AST from current node depth first
void
GetNextChildNodeDepthFirst(
    const tASTNodeLocation& inCurrNodePath,
    tASTNodeLocation&       outNextNodePath)
{
    tASTNodeLocation::const_reverse_iterator rparent_it(inCurrNodePath.rbegin());
    TIntermNode* next_node(nullptr);
    TIntermNode* child_node(nullptr);
    do
    {
        next_node = FindNextChildNodeDepthFirst::sGetNextChildNode(*rparent_it, child_node);
        child_node = *rparent_it;
        ++rparent_it;
    }
    while (next_node == nullptr && rparent_it != inCurrNodePath.rend());

    if (next_node != nullptr)
    {
        // Copy parent node path
        outNextNodePath.assign(inCurrNodePath.begin(), (rparent_it - 1).base());
        outNextNodePath.push_back(next_node);

        AssertNodeLocationDirectParentChild(outNextNodePath);
    }
}


// Tests if child node is a direct descendent of the parent
bool
IsDirectChildNode(TIntermNode* inParentNode, TIntermNode* inChildNode)
{
    return DirectChildTester::sIsDirectChildNode(inParentNode, inChildNode);
}


// Verify that  consecutive nodes have a direct parent - child relation
void
AssertNodeLocationDirectParentChild(const tASTNodeLocation& inNodeLocation)
{
#if !defined(NDEBUG)
    size_t node_count(inNodeLocation.size());
    assert(node_count>= 2);
    for (size_t n_ix(0); n_ix < node_count - 1; ++n_ix)
    {
        assert(IsDirectChildNode(inNodeLocation[n_ix], inNodeLocation[n_ix + 1]));
    }
#endif
}
