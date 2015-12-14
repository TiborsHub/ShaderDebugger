/*******************************************************************************\
*																				*
*			Class   : InspectContextSingleTriangle	                            *
*			Purpose : Interface to program and all state to needed to run       *
*                     program                                                   *
*			File    : InspectContextSingleTriangle.cpp						    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "InspectContextSingleTriangle.h"


// ANGLE headers
#include <shader_utils.h>


// Project headers
#include "../AngleUtil/Win32EmbeddedWindow.h"


// Standard headers
#include <limits>
#include <assert.h>


// Constructor
InspectContextSingleTriangle::InspectContextSingleTriangle()
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
        \n    float sin_r_arg = 10.0 * vPosition.x;
        \n    float color_r = max(sin(sin_r_arg), 0.0);
        \n    float sin_g_arg = 20.0 * vPosition.y;
        \n    float color_g = max(sin(sin_g_arg), 0.0);
        \n    gl_FragColor = vec4(color_r, color_g, vPosition.z, 1.0);
        \n}
    );

    mVertices =
    {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
}


