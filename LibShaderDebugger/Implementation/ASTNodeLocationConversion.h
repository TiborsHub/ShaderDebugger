/*******************************************************************************\
*                                                                               *
*           Utility : ASTNodeLocationConversion                                 *
*           Purpose : Functions to convert from/to a tree independent node      *
*                     path description                                          *
*           File    : ASTNodeLocationConversion.h                               *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __AST_NODE_LOCATION_CONVERSION_H__
#define __AST_NODE_LOCATION_CONVERSION_H__


// Forward declarations
class TIntermNode;


// Library headers
#include "ASTLocation.h"


// Return path in node tree by depth and index
void
GetNodeIndexPath(
    const std::vector<TIntermNode*>& inNodePath,
    tASTLocation&                    outNodeIndexPath);


// Return path to node as list of direct child nodes
bool
GetNodePath(
    TIntermNode*               inAST,
    const tASTLocation&        inNodeIndexPath,
    std::vector<TIntermNode*>& outNodePath);


#endif // __AST_NODE_LOCATION_CONVERSION_H__
