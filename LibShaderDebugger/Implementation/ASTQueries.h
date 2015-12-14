/*******************************************************************************\
*																				*
*			Utility : ASTQueries					                                *
*			Purpose : Queries to compute certain properties of an AST           *
*			File    : ASTQueries	.h							                    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __AST_QUERIES_H__
#define __AST_QUERIES_H__


// Forward declarations
class TIntermNode;
class TIntermAggregate;
class TIntermSymbol;


// ANGLE headers
#include <compiler/translator/Operator.h>


// Standard headers
#include <vector>


// Returns operator if node is a binary node
TOperator
GetBinaryOperator(size_t inNodeIx, const std::vector<TIntermNode*>& inSymbolNodePath);


// Returns true if target node is part of a declaration
bool
IsInDeclaration(const std::vector<TIntermNode*>& inSymbolNodePath);


// Return true if target node is part of an assignment
bool
IsInAssignment(const std::vector<TIntermNode*>& inSymbolNodePath);


// Find sequence statement which the direct parent of the target (last) node
int
FindDirectParentSequence(const std::vector<TIntermNode*>& inSymbolNodePath);


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
FindValueExpressionNode(const std::vector<TIntermNode*>& inSymbolNodePath);


#endif // __AST_QUERIES_H__
