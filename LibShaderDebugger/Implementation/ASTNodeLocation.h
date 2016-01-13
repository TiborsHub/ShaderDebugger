/*******************************************************************************\
*                                                                               *
*           Typedef : ASTNodeLocation                                           *
*           Purpose : Type definition for node location as list of child nodes  *
*           File    : ASTNodeLocation.h                                         *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __AST_NODE_LOCATION_H__
#define __AST_NODE_LOCATION_H__


// Forward declaration
class TIntermNode;


// Standard headers
#include <vector>


// Typedef for node location as list of child nodes
// [0] : root node
typedef std::vector<TIntermNode*> tASTNodeLocation;


#endif // __AST_NODE_LOCATION_H__
