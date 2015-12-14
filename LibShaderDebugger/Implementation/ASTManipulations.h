/*******************************************************************************\
*																				*
*			Utility : ASTManipulations					                        *
*			Purpose : Utility functions to change / extend part of the AST      *
*			File    : ASTManipulations.h							                *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __AST_MANIPULATIONS_H__
#define __AST_MANIPULATIONS_H__


// Forward declarations
class TIntermSymbol;
class TIntermTyped;
class TIntermBranch;
class TParseContext;


// ANGLE headers
#include <compiler/translator/Common.h>


// Standard headers
#include <string>


// Create node tree for assignment of a vector of size n (max 4) to another
TIntermTyped*
CreateAssignmentVecN(
    TIntermTyped*  inLeftNode,
    TIntermTyped*  inRightNode,
    TParseContext& inParseContext);


// Create return from function statement
TIntermBranch*
CreateReturnStatement();


// Return member list for component selection
// Names are taken from the sequence { x, y, z, w } starting at x
TString
GetConsecutiveComponentList(size_t inFieldCount);


#endif // __AST_MANIPULATIONS_H__
