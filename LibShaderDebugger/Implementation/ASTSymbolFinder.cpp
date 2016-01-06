/*******************************************************************************\
*                                                                               *
*           Class   : ASTSymbolFinder                                           *
*           Purpose : AST traverser which searches for the node which contains  *
*                     the target symbol                                         *
*           File    : ASTSymbolFinder.cpp                                       *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "ASTSymbolFinder.h"


// Constructor
ASTSymbolFinder::ASTSymbolFinder(
    const std::string& inSymbol,
    int                inLine,
    int                inColumn)
:
    TIntermTraverser (true, false, false),
    mSymbol          (inSymbol),
    mLine            (inLine),
    mColumn          (inColumn)
{

}


// Visit symbol type node
// virtual
void
ASTSymbolFinder::visitSymbol(TIntermSymbol* inNode)
{
    if (inNode->getLine().first_line == mLine && inNode->getSymbol().c_str() == mSymbol)
    {
        assert(inNode->getAsSymbolNode() != nullptr);

        // mPath is path from root ([0]) up to including parent of target node
        mPathToSymbolNode.assign(mPath.begin(), mPath.end());

        // Add target node
        mPathToSymbolNode.push_back(inNode);
    }
}
