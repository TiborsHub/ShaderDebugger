/*******************************************************************************\
*																				*
*			Utility : AddTestCasesWebGL1                                        *
*			Purpose : Add WebGL 1.0 test cases to test case factory singleton   *
*			File    : AddTestCasesWebGL1.cpp							            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "AddTestCasesWebGL1.h"


// Project headers
#include "TestCaseFactorySingleton.h"
#include "TestCaseFactory.h"

// Test cases
#include "InspectContextTestCode.h"
#include "InspectContextValueTest.h"
#include "InspectContextSingleTriangle.h"
#include "InspectContextFbReadTest.h"


// Add test cases for WebGL 1.0
void
AddTestCasesWebGL1()
{
    gTestCaseFactorySingleton->AddTestCase("Single triangle",
        []() -> SpInspectContextI { return std::make_shared<InspectContextSingleTriangle>(); });

    gTestCaseFactorySingleton->AddTestCase("Test code",
        []() -> SpInspectContextI { return std::make_shared<InspectContextTestCode>(); });

    gTestCaseFactorySingleton->AddTestCase("Test reading floating point data from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextFbReadTest>(); });

    gTestCaseFactorySingleton->AddTestCase("Test reading red color from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextValueTest>("vec4(1.0, 0.0, 0.0, 0.4);"); });

    gTestCaseFactorySingleton->AddTestCase("Test reading green color from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextValueTest>("vec4(0.0, 1.0, 0.0, 0.4);"); });

    gTestCaseFactorySingleton->AddTestCase("Test reading blue color from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextValueTest>("vec4(0.0, 0.0, 1.0, 0.4);"); });

    gTestCaseFactorySingleton->AddTestCase("Test reading alpha color from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextValueTest>("vec4(1.0, 0.0, 0.0, 1.0);"); });

    gTestCaseFactorySingleton->AddTestCase("Test reading device coords from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextValueTest>("vec4(gl_FragCoord.xy, vPosition.xy);"); });

    gTestCaseFactorySingleton->AddTestCase("Test fade red to bottom",
        []() -> SpInspectContextI { return std::make_shared<InspectContextValueTest>("vec4(0.5 * (vPosition.y + 1.0), 0.0, 0.0, 1.0);"); });
}
