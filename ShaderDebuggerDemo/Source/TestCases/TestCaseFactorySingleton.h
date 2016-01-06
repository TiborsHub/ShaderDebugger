/*******************************************************************************\
*                                                                               *
*           Utility : TestCaseFactorySingleton                                  *
*           Purpose : The single test case factory instance                     *
*           File    : TestCaseFactorySingleton.h                                *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __TEST_CASE_FACTORY_SINGLETON_H__
#define __TEST_CASE_FACTORY_SINGLETON_H__


// Forward declarations
class TestCaseFactory;


// The single instance
extern
TestCaseFactory*
gTestCaseFactorySingleton;


// Create singleton
bool
CreateTestCaseFactorySingleton();


// Destroy singleton
void
DestroyTestCaseFactorySingleton();


#endif // __TEST_CASE_FACTORY_SINGLETON_H__
