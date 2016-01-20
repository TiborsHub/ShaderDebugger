/*******************************************************************************\
*                                                                               *
*           Utility : ASTNodeLocationConversion                                 *
*           Purpose : Functions to convert from/to a tree independent node      *
*                     path description                                          *
*           File    : ASTNodeLocationConversion.cpp                             *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "ASTNodeLocationConversion.h"


// ANGLE headers
#include <compiler/translator/IntermNode.h>


namespace
{

// Utility class to compute child node index
class NodeIndexPathFromNodePath: public TIntermTraverser
{
public:
                                // Constructor
                                NodeIndexPathFromNodePath(TIntermNode* inTargetChildNode);

                                // Returns index of direct child node
    int                         GetChildIndex() const;

                                // Compute index in list of direct children according to traverse order
    static int                  sGetChildNodeIndex(TIntermNode* inParentNode, TIntermNode* inChildNode);

private:
                                // Visit of terminal nodes
    virtual void                visitSymbol        (TIntermSymbol*        inNode) override { visitTerminalNode(inNode); }
    virtual void                visitRaw           (TIntermRaw*           inNode) override { visitTerminalNode(inNode); }
    virtual void                visitConstantUnion (TIntermConstantUnion* inNode) override { visitTerminalNode(inNode); }

                                // Visit of non-terminal nodes
    virtual bool                visitBinary    (Visit inVisit, TIntermBinary*    inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitUnary     (Visit inVisit, TIntermUnary*     inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitSelection (Visit inVisit, TIntermSelection* inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitSwitch    (Visit inVisit, TIntermSwitch*    inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitCase      (Visit inVisit, TIntermCase*      inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitAggregate (Visit inVisit, TIntermAggregate* inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitLoop      (Visit inVisit, TIntermLoop*      inNode) override { return visitNonTerminalNode(inVisit, inNode); }
    virtual bool                visitBranch    (Visit inVisit, TIntermBranch*    inNode) override { return visitNonTerminalNode(inVisit, inNode); }

                                // Common implementation for terminal nodes
    void                        visitTerminalNode(TIntermNode* inNode);

                                // Common implementation of non terminal nodes
    bool                        visitNonTerminalNode(Visit inVisit, TIntermNode* inNode);

    TIntermNode*                mTargetChildNode;
    int                         mCurrentChildNodeIx;
    int                         mTargetChildNodeIx;
};


// Constructor
NodeIndexPathFromNodePath::NodeIndexPathFromNodePath(TIntermNode* inTargetChildNode) :
    mTargetChildNode    (inTargetChildNode),
    mCurrentChildNodeIx (-1),
    mTargetChildNodeIx  (-1),
    TIntermTraverser    (true, false, false) // pre visit, visit, post visit
{

}


// Returns index of direct child node
int
NodeIndexPathFromNodePath::GetChildIndex() const
{
    assert(mTargetChildNodeIx != -1);

    return mTargetChildNodeIx;
}


// Common implementation for terminal nodes
void
NodeIndexPathFromNodePath::visitTerminalNode(TIntermNode* inNode)
{
    if (mDepth == 1)
    {
        ++mCurrentChildNodeIx;
        if (inNode == mTargetChildNode)
        {
            assert(mTargetChildNodeIx == -1);
            mTargetChildNodeIx = mCurrentChildNodeIx;
        }
    }
}


// Common implementation of non terminal nodes
bool
NodeIndexPathFromNodePath::visitNonTerminalNode(Visit inVisit, TIntermNode* inNode)
{
    if (mDepth == 1)
    {
        ++mCurrentChildNodeIx;
        if (inNode == mTargetChildNode)
        {
            assert(mTargetChildNodeIx == -1);
            mTargetChildNodeIx = mCurrentChildNodeIx;
        }
    }

    return (mTargetChildNodeIx == -1) && (mDepth <= 1);
}


// Compute index in list of direct children according to traverse order
//static
int
NodeIndexPathFromNodePath::sGetChildNodeIndex(TIntermNode* inParentNode, TIntermNode* inChildNode)
{
    NodeIndexPathFromNodePath find_child_node_ix(inChildNode);
    inParentNode->traverse(&find_child_node_ix);

    return find_child_node_ix.GetChildIndex();
}


// Utility class to compute direct child node for given index
class NodePathFromIndexPath : public TIntermTraverser
{
public:
                                // Constructor
                                NodePathFromIndexPath(int inChildIndex);

                                // Returns child node
    TIntermNode*                GetChildNode() const;

                                // Compute direct child node at index according to traverse order
    static TIntermNode*         sGetChildNode(TIntermNode* inParentNode, int inChildIndex);

protected:

                                // Visit of terminal nodes
    virtual void                visitSymbol        (TIntermSymbol*        inNode) override { visitTerminalNode(inNode); }
    virtual void                visitRaw           (TIntermRaw*           inNode) override { visitTerminalNode(inNode); }
    virtual void                visitConstantUnion (TIntermConstantUnion* inNode) override { visitTerminalNode(inNode); }

                                // Visit of non-terminal nodes
    virtual bool                visitBinary        (Visit, TIntermBinary*    inNode) override { return visitNonTerminalNode(inNode); }
    virtual bool                visitUnary         (Visit, TIntermUnary*     inNode) override { return visitNonTerminalNode(inNode); }
    virtual bool                visitSelection     (Visit, TIntermSelection* inNode) override { return visitNonTerminalNode(inNode); }
    virtual bool                visitSwitch        (Visit, TIntermSwitch*    inNode) override { return visitNonTerminalNode(inNode); }
    virtual bool                visitCase          (Visit, TIntermCase*      inNode) override { return visitNonTerminalNode(inNode); }
    virtual bool                visitAggregate     (Visit, TIntermAggregate* inNode) override { return visitNonTerminalNode(inNode); }
    virtual bool                visitLoop          (Visit, TIntermLoop*      inNode) override { return visitNonTerminalNode(inNode); }
    virtual bool                visitBranch        (Visit, TIntermBranch*    inNode) override { return visitNonTerminalNode(inNode); }

private:
                                // Common implementation for terminal nodes
    void                        visitTerminalNode(TIntermNode* inNode);

                                // Common implementations for non terminal nodes
    bool                        visitNonTerminalNode(TIntermNode* inNode);

    TIntermNode*                mTargetChildNode;
    int                         mCurrentChildNodeIx;
    int                         mTargetChildNodeIx;
};


// Constructor
NodePathFromIndexPath::NodePathFromIndexPath(int inChildIndex)
:
    mTargetChildNode    (nullptr),
    mCurrentChildNodeIx (-1),
    mTargetChildNodeIx  (inChildIndex),
    TIntermTraverser    (true, false, false) // pre visit, visit, post visit
{

}


// Returns child node
TIntermNode*
NodePathFromIndexPath::GetChildNode() const
{
    assert(mTargetChildNode != nullptr);

    return mTargetChildNode;
}


// Common implementation for terminal nodes
void
NodePathFromIndexPath::visitTerminalNode(TIntermNode* inNode)
{
    if (mDepth == 1)
    {
        ++mCurrentChildNodeIx;
        if (mCurrentChildNodeIx == mTargetChildNodeIx)
        {
            assert(mTargetChildNode == nullptr);

            mTargetChildNode = inNode;
        }
    }
}


// Common implementations for non terminal nodes
bool
NodePathFromIndexPath::visitNonTerminalNode(TIntermNode* inNode)
{
    if (mDepth == 1)
    {
        ++mCurrentChildNodeIx;
        if (mCurrentChildNodeIx == mTargetChildNodeIx)
        {
            assert(mTargetChildNode == nullptr);

            mTargetChildNode = inNode;
        }
    }

    return (mTargetChildNode == nullptr) && (mDepth <= 1);
}


// Compute direct child node at index according to traverse order
// static
TIntermNode*
NodePathFromIndexPath::sGetChildNode(
    TIntermNode* inParentNode,
    int          inChildIndex)
{
    NodePathFromIndexPath find_child_node(inChildIndex);
    inParentNode->traverse(&find_child_node);

    return find_child_node.GetChildNode();
}

}; // namespace


// Return path in node tree by depth and index
void
GetNodeIndexPath(
    const tASTNodeLocation& inNodePath,
    tASTLocation&           outNodeIndexPath)
{
    assert(inNodePath.size() >= 2);
    size_t child_node_count(inNodePath.size() - 1);
    outNodeIndexPath.resize(child_node_count);
    for (size_t n_ix(0); n_ix < child_node_count; ++n_ix)
    {
        int child_node_ix(NodeIndexPathFromNodePath::sGetChildNodeIndex(inNodePath[n_ix], inNodePath[n_ix + 1]));
        assert(child_node_ix != -1);

        outNodeIndexPath[n_ix] = child_node_ix;
    }
}


// Return path in node tree by depth and index for a list of node paths
void
GetNodeIndexPaths(
    const std::vector<tASTNodeLocation>& inNodePath,
    std::vector<tASTLocation>&           outNodeIndexPath)
{
    outNodeIndexPath.resize(inNodePath.size());
    for (auto l_it(inNodePath.begin()); l_it != inNodePath.end(); ++l_it)
    {
        GetNodeIndexPath(*l_it, outNodeIndexPath[l_it - inNodePath.begin()]);
    }
}


// Return path to node as list of direct child nodes
bool
GetNodePath(
    TIntermNode*        inAST,
    const tASTLocation& inNodeIndexPath,
    tASTNodeLocation&   outNodePath)
{
    assert(!inNodeIndexPath.empty());
    assert(outNodePath.empty());

    outNodePath.push_back(inAST);
    for (size_t n_ix(0); n_ix < inNodeIndexPath.size(); ++n_ix)
    {
        TIntermNode* child_node(NodePathFromIndexPath::sGetChildNode(outNodePath[n_ix], inNodeIndexPath[n_ix]));
        assert(child_node != nullptr);

        outNodePath.push_back(child_node);
    }

    return !outNodePath.empty();
}


// Return path to node as list of direct child nodes for a list of child node index locations
bool
GetNodePaths(
    TIntermNode*                     inAST,
    const std::vector<tASTLocation>& inNodeIndexPaths,
    std::vector<tASTNodeLocation>&   outNodePaths)
{
    for (auto l_it(inNodeIndexPaths.begin()); l_it != inNodeIndexPaths.end(); ++l_it)
    {
        if (!GetNodePath(inAST, *l_it, outNodePaths[l_it - inNodeIndexPaths.begin()]))
        {
            return false;
        }
    }

    return true;
}
