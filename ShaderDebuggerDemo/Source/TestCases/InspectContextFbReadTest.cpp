/*******************************************************************************\
*																				*
*			Class   : InspectContextFbReadTest	                                *
*			Purpose : Test the reading from the frambuffer                      *
*			File    : InspectContextFbReadTest.cpp						        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "InspectContextFbReadTest.h"


// ANGLE headers
#include <shader_utils.h>


// Standard headers
#include <limits>
#include <assert.h>


// Constructor
InspectContextFbReadTest::InspectContextFbReadTest()
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
        \n    vec3 color = vPosition.xyz;
        \n    vec3 color_abs = abs(color);
        \n    vec3 color_max = max(color, vec3(0.0, 0.0, 0.0));

        \n    gl_FragColor = vec4(color_abs, 1.0);
        \n    return;
        \n}
    );
}


