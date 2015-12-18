/*******************************************************************************\
*																				*
*			Utility : ASTManipulations					                        *
*			Purpose : Utility functions to change / extend part of the AST      *
*			File    : ASTManipulations.cpp							            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "ASTManipulations.h"


// Library headers
#include "ASTQueries.h"


// ANGLE headers
#include <compiler/translator/Compiler.h>
#include <compiler/translator/Intermediate.h>
#include <compiler/translator/ParseContext.h>


// Create node tree for assignment of a vector of size n (max 4) to another
TIntermTyped*
CreateAssignmentVecN(
    TIntermTyped*  inLeftNode,
    TIntermTyped*  inRightNode,
    TParseContext& inParseContext)
{
    TIntermTyped* left_node(inLeftNode);
    TSourceLoc    src_loc = {0, 0, 0, 0};

    // Discover size of target
    int left_size(GetScalarCountFromExpression(inLeftNode));
    int right_size(GetScalarCountFromExpression(inRightNode));

    if (left_size != right_size)
    {
        // Insert swizzle node
        TString fields(GetConsecutiveComponentList(right_size));
        left_node = inParseContext.addFieldSelectionExpression(
            inLeftNode,
            src_loc,
            fields,
            src_loc);
    }

    // Construct assignment
    TIntermTyped* assign_stmt(inParseContext.addAssign(
        EOpAssign,
        left_node,
        inRightNode,
        src_loc));

    return assign_stmt;
}


// Create return from function statement
TIntermBranch*
CreateReturnStatement()
{
    TInfoSink info_sink;
    TIntermediate node_factory(info_sink);
    TSourceLoc src_loc = {0, 0, 0, 0};

    return node_factory.addBranch(EOpReturn, src_loc);
}


// Return member list for component selection
// Names are taken from the sequence { x, y, z, w } starting at x
TString
GetConsecutiveComponentList(size_t inFieldCount)
{
    assert(inFieldCount <= 4);

    return TString("xyzw", inFieldCount);
}
