/*******************************************************************************\
*                                                                               *
*           Utility : TestCaseFactorySingleton                                  *
*           Purpose : The single test case factory instance                     *
*           File    : TestCaseFactorySingleton.cpp                              *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "TestCaseFactorySingleton.h"


// Project headers
#include "TestCaseFactory.h"


// Standard headers
#include <assert.h>


// Application wide instance
TestCaseFactory*
gTestCaseFactorySingleton(nullptr);


// Create singleton
bool
CreateTestCaseFactorySingleton()
{
    assert(gTestCaseFactorySingleton == nullptr);

    gTestCaseFactorySingleton = new TestCaseFactory;
    return (gTestCaseFactorySingleton != nullptr);
}


// Destroy singleton
void
DestroyTestCaseFactorySingleton()
{
    assert(gTestCaseFactorySingleton != nullptr);

    delete gTestCaseFactorySingleton;
    gTestCaseFactorySingleton = nullptr;
}
