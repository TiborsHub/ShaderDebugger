/*******************************************************************************\
*                                                                               *
*           Utility : AddTestCasesWebGL1                                        *
*           Purpose : Add WebGL 1.0 test cases to test case factory singleton   *
*           File    : AddTestCasesWebGL1.cpp                                    *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "AddTestCasesWebGL1.h"


// Project headers
#include "TestCaseFactorySingleton.h"
#include "TestCaseFactory.h"

// Test cases
#include "InspectContextWebGL1Basic.h"


// ANGLE headers
#include <shader_utils.h>


namespace
{
    // Triangle vertex list
    std::vector<GLfloat> triangle_vertices =
    {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // Triangle strip covering the complete viewport
    std::vector<GLfloat> viewport_quad_vertices =
    {
        -1.0f,  1.0f, 0.0f, // Left  top
        -1.0f, -1.0f, 0.0f, // Left  bottom
         1.0f,  1.0f, 0.0f, // Right top
         1.0f, -1.0f, 0.0f, // Right bottom
    };

    std::string vertex_shader_pos_varying = SHADER_SOURCE
    (
          attribute vec4 aPosition;
        \nvarying vec4 vPosition;
        \nvoid main()
        \n{
        \n    vPosition = aPosition;
        \n    gl_Position = aPosition;
        \n}
    );

}


// Add test cases for WebGL 1.0
void
AddTestCasesWebGL1()
{
    // Single triangle
    gTestCaseFactorySingleton->AddTestCase(
        "Single triangle",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
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
            ),
            triangle_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test code
    gTestCaseFactorySingleton->AddTestCase(
        "Test code",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                \n varying vec4 vPosition;
                \nvoid main()
                \n{
                \n    gl_FragColor.yz = vPosition.xw;
                \n    gl_FragColor.w = vPosition[1];
                \n}
            ),
            triangle_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test reading floating point data from frame buffer
    gTestCaseFactorySingleton->AddTestCase(
        "Test reading floating point data from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
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
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test reading red color from frame buffer
    gTestCaseFactorySingleton->AddTestCase(
        "Test reading red color from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                  // Input
                \nvarying vec4 vPosition;

                \nvoid main()
                \n{
                \n    gl_FragColor = vec4(1.0, 0.0, 0.0, 0.4);
                \n}
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test reading green color from frame buffer
    gTestCaseFactorySingleton->AddTestCase(
        "Test reading green color from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                  // Input
                \nvarying vec4 vPosition;

                \nvoid main()
                \n{
                \n    gl_FragColor = vec4(0.0, 1.0, 0.0, 0.4);
                \n}
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test reading blue color from frame buffer
    gTestCaseFactorySingleton->AddTestCase(
        "Test reading blue color from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                  // Input
                \nvarying vec4 vPosition;

                \nvoid main()
                \n{
                \n    gl_FragColor = vec4(0.0, 0.0, 1.0, 0.4);
                \n}
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test reading alpha color from frame buffer
    gTestCaseFactorySingleton->AddTestCase(
        "Test reading alpha color from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                  // Input
                \nvarying vec4 vPosition;

                \nvoid main()
                \n{
                \n    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.4);
                \n}
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test reading device coords from frame buffer
    gTestCaseFactorySingleton->AddTestCase(
        "Test reading device coords from frame buffer",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                  // Input
                \nvarying vec4 vPosition;

                \nvoid main()
                \n{
                \n    gl_FragColor = vec4(gl_FragCoord.xy, vPosition.xy);
                \n}
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test fade red to bottom
    gTestCaseFactorySingleton->AddTestCase(
        "Test fade red to bottom",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                  // Input
                \nvarying vec4 vPosition;

                \nvoid main()
                \n{
                \n    gl_FragColor = vec4(0.5 * (vPosition.y + 1.0), 0.0, 0.0, 1.0);
                \n}
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test if statement
    gTestCaseFactorySingleton->AddTestCase(
        "Test if statement",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                  // Input
                \nvarying vec4 vPosition;

                \nvoid main()
                \n{
                \n    if (mod(gl_FragCoord.x - 0.5, 2.0) == 0.0)
                \n    {
                \n        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
                \n    }
                \n    else
                \n    {
                \n        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
                \n    }
                \n}
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });

    // Test shader with loop, function call and selection
    gTestCaseFactorySingleton->AddTestCase(
        "Test shader with loop, function call and selection",
        []() -> SpInspectContextI { return std::make_shared<InspectContextWebGL1Basic>(
            vertex_shader_pos_varying,
            SHADER_SOURCE
            (
                  precision mediump float;

                  // Input
                \nvarying vec4 vPosition;

                \nfloat Sum(in float inCount)
                \n{
                \n    float sum = 0.0;
                \n    for (float ix = 0.0; ix < 100.0; ++ix)
                \n    {
                \n        sum += sin(ix);
                \n        if (ix >= inCount)
                \n        {
                \n            break;
                \n        }
                \n    }
                \n
                \n    return sum;
                \n}

                \nvoid main()
                \n{
                \n    if (mod(gl_FragCoord.x - 0.5, 2.0) == 0.0)
                \n    {
                \n        gl_FragColor = vec4(1.0, Sum(gl_FragCoord.x - 0.5), gl_FragCoord.x, 1.0);
                \n    }
                \n    else
                \n    {
                \n        gl_FragColor = vec4(Sum(gl_FragCoord.y - 0.5), 1.0, gl_FragCoord.x, 1.0);
                \n    }
                \n}
            ),
            viewport_quad_vertices,
            GL_TRIANGLE_STRIP);
        });
}
