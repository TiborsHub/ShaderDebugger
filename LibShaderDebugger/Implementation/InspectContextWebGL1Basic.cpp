/*******************************************************************************\
*																				*
*			Class   : InspectContextWebGL1Basic	                                *
*			Purpose : Inspect context with default functionality to render      *
*                     one vertex buffer and update the viewport                 *
*			File    : InspectContextWebGL1Basic.cpp							    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "InspectContextWebGL1Basic.h"


// Constructor
InspectContextWebGL1Basic::InspectContextWebGL1Basic()
{
    // Triangle strip covering the complete viewport
    mVertices =
    {
        -1.0f,  1.0f, 0.0f, // Left  top
        -1.0f, -1.0f, 0.0f, // Left  bottom
         1.0f,  1.0f, 0.0f, // Right top
         1.0f, -1.0f, 0.0f, // Right bottom
    };
}


// Run the program which is the subject of the inspection
// virtual
bool
InspectContextWebGL1Basic::RunTargetProgram()
{
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Load the vertex data
    const size_t vx_comp_count(3);
    const size_t vx_count(mVertices.size() / vx_comp_count);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vx_comp_count, GL_FLOAT, GL_FALSE, 0, mVertices.data());

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vx_count);

    return (glGetError() == GL_NO_ERROR);
}


