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
#include <cassert>
#include <climits>


namespace
{

// Utility class to compute node type
class NodeTypeEvaluator : public TIntermTraverser
{
public:
                                // Constructor
                                NodeTypeEvaluator() :
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


// Utility class to find next child in traverse order
class FindNextChildNode : public TIntermTraverser
{
public:
                                // Constructor
                                // If target child node is not null the next node is the first node from the parent
                                // after the target child node
                                // Otherwise it is the first child node from the parent
                                FindNextChildNode(TIntermNode* inTargetChildNode);

                                // Return next child node
    TIntermNode*                GetNextChildNode() const { return mNextChildNode; }

                                // Return next child node
                                // If target child node is not null the next node is the first node from the parent
                                // after the target child node
                                // Otherwise it is the first child node from the parent
    static TIntermNode*         sGetNextChildNode(TIntermNode* inParentNode, TIntermNode* inTargetChildNode);

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
    TIntermNode*                mNextChildNode;
};


// Constructor
FindNextChildNode::FindNextChildNode(TIntermNode* inTargetChildNode) :
    TIntermTraverser (true, false, false), // pre visit, visit, post visit
    mTargetChildNode (inTargetChildNode),
    mNextChildNode   (nullptr)
{

}


// Common implementation for terminal nodes
void
FindNextChildNode::visitTerminalNode(TIntermNode* inNode)
{
    if (mDepth == 1)
    {
        if (mTargetChildNode == nullptr)
        {
            if (mNextChildNode == nullptr)
            {
                mNextChildNode = inNode;
            }
        }
        else
        {
            if (inNode == mTargetChildNode)
            {
                mTargetChildNode = nullptr;
            }
        }
    }
}


// Common implementation of non terminal nodes
bool
FindNextChildNode::visitNonTerminalNode(Visit inVisit, TIntermNode* inNode)
{
    (void)inVisit;

    visitTerminalNode(inNode);

    return (mNextChildNode == nullptr && mDepth <= 1);
}


// Return next child node
//static
TIntermNode*
FindNextChildNode::sGetNextChildNode(TIntermNode* inParentNode, TIntermNode* inTargetChildNode)
{
    if (inTargetChildNode != nullptr)
    {
        assert(IsDirectChildNode(inParentNode, inTargetChildNode));
    }

    FindNextChildNode find_next_child_node(inTargetChildNode);
    inParentNode->traverse(&find_next_child_node);
    TIntermNode* next_child_node(find_next_child_node.GetNextChildNode());

    if (next_child_node != nullptr)
    {
        assert(IsDirectChildNode(inParentNode, next_child_node));
    }

    return next_child_node;
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
    NodeTypeEvaluator node_type_eval;
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


namespace
{
    TOperator sAssignmentOperators[] =
    {
        // Assignment
        EOpAssign,
        EOpInitialize,
        EOpAddAssign,
        EOpSubAssign,

        EOpMulAssign,
        EOpVectorTimesMatrixAssign,
        EOpVectorTimesScalarAssign,
        EOpMatrixTimesScalarAssign,
        EOpMatrixTimesMatrixAssign,

        EOpDivAssign,
        EOpIModAssign,
        EOpBitShiftLeftAssign,
        EOpBitShiftRightAssign,
        EOpBitwiseAndAssign,
        EOpBitwiseXorAssign,
        EOpBitwiseOrAssign
    };
}


// Returns true if operator is an assignment
bool
IsAssignmentOperator(TOperator inOperator)
{
    for (size_t s_ix(0); s_ix < sizeof(sAssignmentOperators) / sizeof(sAssignmentOperators[0]); ++s_ix)
    {
        if (inOperator == sAssignmentOperators[s_ix])
        {
            return true;
        }
    }

    return false;
}


// Returns true if node is function call
bool
IsFunctionCall(TIntermNode* inNode)
{
    TIntermAggregate* aggregate(inNode->getAsAggregate());
    return (aggregate != nullptr && aggregate->getOp() == EOpFunctionCall);
}


// Return next child node in traverse order
// Return child of leave node if it exist
// Otherwise return next child of parent of leave node
void
GetNextChildNode(const tASTNodeLocation& inCurrNodePath, tASTNodeLocation& outNextNodePath)
{
    AssertNodeLocationDirectParentChild(inCurrNodePath);

    tASTNodeLocation::const_reverse_iterator rparent_it(inCurrNodePath.rbegin());
    TIntermNode* child_node(nullptr);

    // Test for child of leave node
    TIntermNode* next_node(FindNextChildNode::sGetNextChildNode(*rparent_it, child_node));
    if (next_node != nullptr)
    {
        outNextNodePath = inCurrNodePath;
        outNextNodePath.push_back(next_node);
    }
    else
    {
        // Iterate over node path
        do
        {
            child_node = *rparent_it;
            ++rparent_it;
            next_node = FindNextChildNode::sGetNextChildNode(*rparent_it, child_node);
        }
        while (next_node == nullptr && rparent_it != inCurrNodePath.rend() - 1);

        if (next_node != nullptr)
        {
            assert(IsDirectChildNode(*rparent_it, next_node));

            // Copy parent node path from root till (including) parent
            tASTNodeLocation::const_iterator parent_it(rparent_it.base() - 1);
            assert(*rparent_it == *parent_it);
            outNextNodePath.assign(inCurrNodePath.begin(), (parent_it + 1));
            assert(*rparent_it == outNextNodePath.back());
            assert(IsDirectChildNode(outNextNodePath.back(), next_node));
            outNextNodePath.push_back(next_node);

            AssertNodeLocationDirectParentChild(outNextNodePath);
        }
    }
}


// Returns index at which two node paths differ
// if std::numeric_limits<size_t>::max() is returned, all nodes from path 1 are equal to the nodes at the same positions
// from path2
// Path 2 can be longer
size_t
FindNodePathDifference(const tASTNodeLocation& inNodePath1, const tASTNodeLocation& inNodePath2)
{
    // Find first node which is different
    for (size_t n_ix(0); n_ix < inNodePath1.size(); ++n_ix)
    {
        if (inNodePath1[n_ix] != inNodePath2[n_ix])
        {
            return n_ix;
        }
    }

    return std::numeric_limits<size_t>::max();
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
