/*******************************************************************************\
*                                                                               *
*           Typedef : ASTLocation                                               *
*           Purpose : Type definition of tree independent node path             *
*                     Location is specified as list of child indices            *
*           File    : ASTLocation.h                                             *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __AST_LOCATION_H__
#define __AST_LOCATION_H__


// Standard headers
#include <vector>


// Typedef for node location as list of child node indices 
// [0] : Child index of direct child from root
typedef std::vector<int> tASTLocation;


#endif // __AST_LOCATION_H__