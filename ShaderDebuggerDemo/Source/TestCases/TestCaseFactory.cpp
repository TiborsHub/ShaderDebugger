/*******************************************************************************\
*																				*
*			Class   : TestCaseFactory	                                        *
*			Purpose : Factory object which creates test cases                   *
*			File    : TestCaseFactory.cpp						                *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "TestCaseFactory.h"


// Standard header
#include <cassert>


// Constructor
TestCaseFactory::TestCaseFactory()
{

}


// Add test case to factory
bool
TestCaseFactory::AddTestCase(const std::string& inTitle, tTestCaseCreator inCreator)
{
    std::pair<tTestCaseMap::iterator, bool> result;
    result = mTestCaseMap.insert(tTestCaseMap::value_type(inTitle, inCreator));

    return result.second;
}


// Create test case for title
SpInspectContextI
TestCaseFactory::CreateTestCase(const std::string& inTitle) const
{
    tTestCaseMap::const_iterator it(mTestCaseMap.find(inTitle));
    assert(it != mTestCaseMap.end());

    return it->second();
}
