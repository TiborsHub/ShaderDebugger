/*******************************************************************************\
*                                                                               *
*           Class   : ASTGetShaderStructureNodes                                *
*           Purpose : Computes nodes at which the shader can take different     *
*                     code paths                                                *
*           File    : ASTGetShaderStructureNodes.h                              *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __AST_GET_SHADER_STRUCTURE_NODES_H__
#define __AST_GET_SHADER_STRUCTURE_NODES_H__


// Forward declarations
class ShaderStructureNodes;
class TIntermNode;


// Return branch nodes from ast
void
GetShaderStructureNodes(TIntermNode* inAST, ShaderStructureNodes& outShaderStructureNodes);


#endif // __AST_GET_SHADER_STRUCTURE_NODES_H__
