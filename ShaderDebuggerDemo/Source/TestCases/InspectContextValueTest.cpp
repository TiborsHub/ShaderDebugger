/*******************************************************************************\
*																				*
*			Class   : InspectContextValueTest	                                *
*			Purpose : Test reading of color channel from the framebuffer        *
*			File    : InspectContextValueTest.cpp						        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "InspectContextValueTest.h"


// ANGLE headers
#include <shader_utils.h>


// Standard headers
#include <limits>
#include <assert.h>


// Constructor
InspectContextValueTest::InspectContextValueTest(const std::string& inColorExpression)
{
    mSourceVertex = SHADER_SOURCE
    (
          // Input
          attribute vec4 aPosition;

          // Output
        \nvarying vec4 vPosition;

        \nvoid main()
        \n{
        \n    vPosition   = aPosition;
        \n    gl_Position = aPosition;
        \n}
    );

    mSourceFragment = SHADER_SOURCE
    (
          precision mediump float;

          // Input
        \nvarying vec4 vPosition;

        \nvoid main()
        \n{
    );

    mSourceFragment += "\n    gl_FragColor = " + inColorExpression;

    mSourceFragment += SHADER_SOURCE(
        \n}
    );
}


