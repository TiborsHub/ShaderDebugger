/*******************************************************************************\
*																				*
*			Class    : ASTPrinter  									            *
*			Purpose  : Prints the ANGLE AST to a stream                         *
*                      Based on \angle\src\compiler\translator\intermOut.cpp    *
*			File     : ASTPrinter.h										        *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#ifndef __AST_PRINTER_H__
#define __AST_PRINTER_H__


// ANGLE headers
#include <compiler/translator/IntermNode.h>


// Standard headers
#include <ostream>


class ASTPrinter : public TIntermTraverser
{
public:
                                // Constructor
                                ASTPrinter(
                                    std::ostream& outStream,
                                    int           inLineNrWidth = 3,
                                    int           inIndentWidth = 2);

                                // Print ast to stream
    static void                 sPrint(
                                    TIntermNode*  inRoot,
                                    std::ostream& outStream,
                                    int           inLineNrWidth = 3,
                                    int           inIndentWidth = 2);

                                // Print ast to string
    static void                 sPrint(
                                    TIntermNode* inRoot,
                                    std::string& outString,
                                    int          inLineNrWidth = 3,
                                    int          inIndentWidth = 2);


protected:
                                // AST node visiting functions
    void                        visitSymbol        (               TIntermSymbol*        inSymbolNode)     override;
    void                        visitConstantUnion (               TIntermConstantUnion* inConstUnionNode) override;
    bool                        visitBinary        (Visit inVisit, TIntermBinary*        inBinaryNode)     override;
    bool                        visitUnary         (Visit inVisit, TIntermUnary*         inUnaryNode)      override;
    bool                        visitSelection     (Visit inVisit, TIntermSelection*     inSelectionNode)  override;
    bool                        visitAggregate     (Visit inVisit, TIntermAggregate*     inAggregateNode)  override;
    bool                        visitLoop          (Visit inVisit, TIntermLoop*          inLoopNode)       override;
    bool                        visitBranch        (Visit inVisit, TIntermBranch*        inBranchNode)     override;

                                // Output prefix for every line to show hierarchy
    void                        OutputLinePrefix(TIntermNode* inNode);

                                // Output stream
    std::ostream&               mStream;
    int                         mLineNrWidth;
    std::string                 mIndentation;
};


#endif // __AST_PRINTER_H__
