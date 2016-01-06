/*******************************************************************************\
*                                                                               *
*           Utility : ASTNodeLocation                                           *
*           Purpose : Functions to convert from/to a tree independent node      *
*                     path description                                          *
*           File    : ASTNodeLocation.h                                         *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __AST_NODE_LOCATION_H__
#define __AST_NODE_LOCATION_H__


// Forward declarations
class TIntermNode;


// Standard headers
#include <vector>


// Return path in node tree by depth and index
void
GetNodeIndexPath(
    const std::vector<TIntermNode*>& inNodePath,
    std::vector<int>&                outNodeIndexPath);


// Return path to node as list of direct child nodes
bool
GetNodePath(
    TIntermNode*               inAST,
    const std::vector<int>&    inNodeIndexPath,
    std::vector<TIntermNode*>& outNodePath);


#endif // __AST_NODE_LOCATION_H__
