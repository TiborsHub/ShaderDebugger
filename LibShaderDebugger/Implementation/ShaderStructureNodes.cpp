/*******************************************************************************\
*                                                                               *
*           Class   : ShaderStructureNodes                                      *
*           Purpose : Information about all nodes at which the shader excution  *
*                     path can deviate from sequential execution                *
*           File    : ShaderStructureNodes.cpp                                  *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "ShaderStructureNodes.h"


namespace
{
    tASTNodeLocation dummy_node_loc;
};


const tASTNodeLocation&
ShaderStructureNodes::GetFunction(const TString& inName)
{
    for (auto f_it(mFunctionDefinitions.begin()); f_it != mFunctionDefinitions.end(); ++f_it)
    {
        TIntermAggregate* aggregate(f_it->back()->getAsAggregate());
        assert(aggregate != nullptr);

        if (aggregate->getName() == inName)
        {
            return *f_it;
        }
    }

    // Function should have been in list
    assert(false);

    return dummy_node_loc;
}
