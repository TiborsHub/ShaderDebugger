/*******************************************************************************\
*                                                                               *
*           Utility : ASTQueries                                                *
*           Purpose : Queries to compute certain properties of an AST           *
*           File    : ASTQueries.h                                              *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __AST_QUERIES_H__
#define __AST_QUERIES_H__


// Forward declarations
class TIntermAggregate;
class TIntermSymbol;


// Library headers
#include "ASTNodeLocation.h"


// ANGLE headers
#include <compiler/translator/Operator.h>


// Node types of ast
enum ASTNodeType
{
    // Utility value to assist in bounds checking
    AST_NODE_TYPE_BEGIN,

    AST_NODE_TYPE_UNKNOWN = AST_NODE_TYPE_BEGIN,

    // Maps to traversable node types
    AST_NODE_TYPE_SYMBOL,
    AST_NODE_TYPE_RAW,
    AST_NODE_TYPE_CONSTANT_UNION,
    AST_NODE_TYPE_BINARY,
    AST_NODE_TYPE_UNARY,
    AST_NODE_TYPE_SELECTION,
    AST_NODE_TYPE_SWITCH,
    AST_NODE_TYPE_CASE,
    AST_NODE_TYPE_AGGREGATE,
    AST_NODE_TYPE_LOOP,
    AST_NODE_TYPE_BRANCH,

    // Utility value to assist in bounds checking
    AST_NODE_TYPE_END
};


// Return type of node
ASTNodeType
GetNodeType(TIntermNode* inNode);


// Returns operator if node is a binary node
TOperator
GetBinaryOperator(size_t inNodeIx, const tASTNodeLocation& inSymbolNodePath);


// Returns true if target node is part of a declaration
bool
IsInDeclaration(const tASTNodeLocation& inSymbolNodePath);


// Return true if target node is part of an assignment
bool
IsInAssignment(const tASTNodeLocation& inSymbolNodePath);


// Find sequence statement which the direct parent of the target (last) node
int
FindDirectParentSequence(const tASTNodeLocation& inSymbolNodePath);


// Find target node in sequence
int
FindNodeinSequence(TIntermNode* inTargetNode, TIntermAggregate* inSequenceNode);


// Return count of scalars from symbol
int
GetScalarCountFromSymbolNode(TIntermSymbol* inSymbolNode);


// Return count of scalars in expression
int
GetScalarCountFromExpression(TIntermNode* inTargetNode);


// Returns top node which makes up the value targeted by the symbol node (front node)
TIntermNode*
FindValueExpressionNode(const tASTNodeLocation& inSymbolNodePath);


// Returns true if operator is an assignment
bool
IsAssignmentOperator(TOperator inOperator);


// Returns true if node is function call
bool
IsFunctionCall(TIntermNode* inNode);


// Return next child node in traverse order
// Return child of leave node if it exist
// Otherwise return next child of parent of leave node
void
GetNextChildNode(const tASTNodeLocation& inCurrNodePath, tASTNodeLocation& outNextNodePath);


// Tests if child node is a direct descendent of the parent
bool
IsDirectChildNode(TIntermNode* inParentNode, TIntermNode* inChildNode);


#if !defined(NDEBUG)

// Verify that consecutive nodes have a direct parent - child relation
void
AssertNodeLocationDirectParentChild(const tASTNodeLocation& inNodeLocation);

#endif

#endif // __AST_QUERIES_H__
