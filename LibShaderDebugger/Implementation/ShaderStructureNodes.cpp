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
    tASTLocation dummy_node_loc;
};


// Return function by name
const tASTLocation&
ShaderStructureNodes::GetFunction(const std::string& inName) const
{
    auto f_it(mFunctionDefinitions.find(inName));

    if (f_it != mFunctionDefinitions.end())
    {
        return f_it->second;
    }
    else
    {
        // Function should have been in list
        assert(false);

        return dummy_node_loc;
    }
}
