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


namespace
{

// Utility class to compute node type
class TIntermNodeTypeEvaluator : public TIntermTraverser
{
public:
                                // Constructor
                                TIntermNodeTypeEvaluator() :
                                    mNodeType        (AST_NODE_TYPE_UNKNOWN),
                                    TIntermTraverser (false, true, false)
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

