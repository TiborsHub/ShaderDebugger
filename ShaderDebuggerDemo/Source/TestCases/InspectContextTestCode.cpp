/*******************************************************************************\
*																				*
*			Class   : InspectContextTestCode	                                    *
*			Purpose : Interface to program and all state needed to run          *
*                     program to test code                                      *
*			File    : InspectContextTestCode.cpp							        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "InspectContextTestCode.h"


// ANGLE headers
#include <shader_utils.h>


// Project headers
#include "../AngleUtil/Win32EmbeddedWindow.h"


// Standard headers
#include <limits>
#include <assert.h>


// Constructor
InspectContextTestCode::InspectContextTestCode()
{
    mSourceVertex = SHADER_SOURCE
    (
          attribute vec4 aPosition;
        \nvarying vec4 vPosition;
        \nvoid main()
        \n{
        \n    vPosition = aPosition;
        \n    gl_Position = aPosition;
        \n}
    );

    mSourceFragment = SHADER_SOURCE
    (
          precision mediump float;

        \n varying vec4 vPosition;
        \nvoid main()
        \n{
        \n    gl_FragColor.yz = vPosition.xw;
        \n    gl_FragColor.w = vPosition[1];
        \n}
    );

    mVertices =
    {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
}


