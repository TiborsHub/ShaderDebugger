/*******************************************************************************\
*																				*
*			Utility : ASTQueries					                                *
*			Purpose : Queries to compute certain properties of an AST           *
*			File    : ASTQueries	.cpp							                    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "ASTQueries.h"


// ANGLE headers
#include <compiler/translator/IntermNode.h>


// Standard headers
#include <assert.h>


// Return type of node
ASTNodeType
GetNodeType(TIntermNode* inNode)
{
    if (inNode->getAsTyped()         != nullptr) return AST_NODE_TYPE_TYPED;
    if (inNode->getAsConstantUnion() != nullptr) return AST_NODE_TYPE_CONSTANT_UNION;
    if (inNode->getAsAggregate()     != nullptr) return AST_NODE_TYPE_AGGREGATE;
    if (inNode->getAsBinaryNode()    != nullptr) return AST_NODE_TYPE_BINARY;
    if (inNode->getAsUnaryNode()     != nullptr) return AST_NODE_TYPE_UNARY;
    if (inNode->getAsSelectionNode() != nullptr) return AST_NODE_TYPE_SELECTION;
    if (inNode->getAsSwitchNode()    != nullptr) return AST_NODE_TYPE_SWITCH;
    if (inNode->getAsCaseNode()      != nullptr) return AST_NODE_TYPE_CASE;
    if (inNode->getAsSymbolNode()    != nullptr) return AST_NODE_TYPE_SYMBOL;
    if (inNode->getAsLoopNode()      != nullptr) return AST_NODE_TYPE_LOOP;
    if (inNode->getAsRawNode()       != nullptr) return AST_NODE_TYPE_RAW;
    
    assert(false);
    return AST_NODE_TYPE_UNKNOWN;
}


// Returns operator if node is a binary node
TOperator
GetBinaryOperator(size_t inNodeIx, const std::vector<TIntermNode*>& inSymbolNodePath)
{
    assert(inNodeIx < inSymbolNodePath.size());

    TIntermBinary* op_node(inSymbolNodePath[inNodeIx]->getAsBinaryNode());
    return (op_node != nullptr) ? op_node->getOp() : EOpNull;
}


// Returns true if target node is part of a declaration
bool
IsInDeclaration(const std::vector<TIntermNode*>& inSymbolNodePath)
{
    TIntermNode* parent(inSymbolNodePath[inSymbolNodePath.size() - 2]);
    TIntermAggregate* op_node(parent->getAsAggregate());

    return (op_node != nullptr) ? (op_node->getOp() == EOpDeclaration) : false;
}


// Return true if target node is part of an assignment
bool
IsInAssignment(const std::vector<TIntermNode*>& inSymbolNodePath)
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
FindDirectParentSequence(const std::vector<TIntermNode*>& inSymbolNodePath)
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
FindValueExpressionNode(const std::vector<TIntermNode*>& inSymbolNodePath)
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

