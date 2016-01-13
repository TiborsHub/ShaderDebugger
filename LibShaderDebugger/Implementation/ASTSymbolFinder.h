/*******************************************************************************\
*                                                                               *
*           Class   : ASTSymbolFinder                                           *
*           Purpose : AST traverser which searches for the node which contains  *
*                     the target symbol                                         *
*           File    : ASTSymbolFinder.h                                         *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __AST_SYMBOL_FINDER_H__
#define __AST_SYMBOL_FINDER_H__


// Library headers
#include "ASTNodeLocation.h"


// ANGLE headers
#include <compiler/translator/IntermNode.h>


class ASTSymbolFinder : public TIntermTraverser
{
public:
                                // Constructor
                                ASTSymbolFinder(
                                    const std::string& inSymbol,
                                    int                inLine,
                                    int                inColumn);

                                // Return path through node tree up to (including) symbol node
                                // result.front() is root node
                                // result.back() is target node
    const tASTNodeLocation&     GetPathToSymbolNode() const;

protected:
                                // Visit symbol type node
    virtual void                visitSymbol(TIntermSymbol* inNode) override;

private:
    std::string                 mSymbol;
    int                         mLine;
    int                         mColumn;
    tASTNodeLocation            mPathToSymbolNode;
};


// Inline definitions

// Return path through node tree up to (including) symbol node
// result.front() is root node
// result.back() is target node
inline
const tASTNodeLocation&
ASTSymbolFinder::GetPathToSymbolNode() const
{
    return mPathToSymbolNode;
}


#endif // __AST_SYMBOL_FINDER_H__
