/*******************************************************************************\
*                                                                               *
*           Class   : TestCaseFactory                                           *
*           Purpose : Factory object which creates test cases                   *
*           File    : TestCaseFactory.h                                         *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __TEST_CASE_FACTORY_H__
#define __TEST_CASE_FACTORY_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(InspectContextI);


typedef
SpInspectContextI (*tTestCaseCreator)();


// Standard headers
#include <map>
#include <string>


class TestCaseFactory
{
public:
                                // Constructor
                                TestCaseFactory();

                                // Add test case to factory
    bool                        AddTestCase(const std::string& inTitle, tTestCaseCreator inCreator);

                                // Return count of test cases
    size_t                      GetTestCaseCount() const;

                                // Create test case for title
    SpInspectContextI           CreateTestCase(const std::string& inTitle) const;

    typedef std::map<const std::string, tTestCaseCreator> tTestCaseMap;
    typedef tTestCaseMap::const_iterator ConstIterator;

                                // Iterator to first test case
    ConstIterator               Begin() const;

                                // Iterator to one beyond last test case
    ConstIterator               End() const;

private:
    tTestCaseMap                mTestCaseMap;
};


// Inline definitions

// Return count of test cases
inline
size_t
TestCaseFactory::GetTestCaseCount() const
{
    return mTestCaseMap.size();
}


// Iterator to first test case
inline
TestCaseFactory::ConstIterator
TestCaseFactory::Begin() const
{
    return mTestCaseMap.begin();
}


// Iterator to one beyond last test case
inline
TestCaseFactory::ConstIterator
TestCaseFactory::End() const
{
    return mTestCaseMap.end();
}


#endif // __TEST_CASE_FACTORY_H__
