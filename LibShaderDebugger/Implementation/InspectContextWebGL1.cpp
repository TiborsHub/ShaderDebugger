/*******************************************************************************\
*																				*
*			Class   : InspectContextWebGL1	                                    *
*			Purpose : Interface to program and all state needed to run a        *
*                     WebGL 1 shader program                                    *
*			File    : InspectContextWebGL1.cpp						            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "InspectContextWebGL1.h"


// Library headers
#include "GlUtil.h"


// ANGLE headers
#include <GLES2/gl2.h>
#include <shader_utils.h>


// Standard headers
#include <limits>
#include <assert.h>


// Constructor
InspectContextWebGL1::InspectContextWebGL1(
    const std::string& inSourceVertex,
    const std::string& inSourceFragment)
:
    mSourceVertex   (inSourceVertex),
    mSourceFragment (inSourceFragment),
    mHandleProgram  (GL_ZERO),
    mHandleVertex   (GL_ZERO),
    mHandleFragment (GL_ZERO)
{
}


// Destructor
// virtual
InspectContextWebGL1::~InspectContextWebGL1()
{
    // GL_ZERO will be silently ignored
    glDeleteProgram(mHandleProgram);
    glDeleteShader(mHandleVertex);
    glDeleteShader(mHandleFragment);
}


// Initialize context
// virtual
bool
InspectContextWebGL1::Initialize(std::string& outText)
{
    mHandleProgram  = glCreateProgram();
    mHandleVertex   = CompileShader(GL_VERTEX_SHADER, mSourceVertex);
    mHandleFragment = CompileShader(GL_FRAGMENT_SHADER, mSourceFragment);

    if (mHandleVertex == GL_ZERO || mHandleFragment == GL_ZERO)
    {
        glDeleteShader(mHandleFragment);
        glDeleteShader(mHandleVertex);
        glDeleteProgram(mHandleProgram);
        mHandleProgram = GL_ZERO;
    }
    else
    {
        glAttachShader(mHandleProgram, mHandleVertex);
        glAttachShader(mHandleProgram, mHandleFragment);

        glLinkProgram(mHandleProgram);

        GLint linkStatus;
        glGetProgramiv(mHandleProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == 0)
        {
            outText += "Program link failed: ";
            AppendProgramInfo(mHandleProgram, outText);

            glDeleteProgram(mHandleProgram);
            mHandleProgram = GL_ZERO;
        }
    }

    return (mHandleProgram != GL_ZERO);
}


// Return count of program parts
// virtual
size_t
InspectContextWebGL1::GetShaderCount() const
{
    return 2;
}


// Return type of
// virtual
uint64_t
InspectContextWebGL1::GetShaderType(size_t inShaderIx) const
{
    switch(inShaderIx)
    {
        case 0: return GL_VERTEX_SHADER;
        case 1: return GL_FRAGMENT_SHADER;

        default:
            assert(false);
            return std::numeric_limits<uint64_t>::max();
    }
}


// Return source of shader
// virtual
std::string
InspectContextWebGL1::GetShaderSource(size_t inShaderIx) const
{
    switch (inShaderIx)
    {
        case 0: return mSourceVertex;
        case 1: return mSourceFragment;

        default:
            assert(false);
            return std::string();
    }
}


// Attach shader to current program
// virtual
uint64_t
InspectContextWebGL1::GetHandle(size_t inShaderIx)
{
    switch (inShaderIx)
    {
        case 0: return mHandleVertex;
        case 1: return mHandleFragment;

        default:
            assert(false);
            return GL_ZERO;
    }
}


// Activate the shader program
// virtual
bool
InspectContextWebGL1::UseProgram()
{
    if (mHandleProgram != GL_ZERO)
    {
        glUseProgram(mHandleProgram);

        return (glGetError() != GL_NO_ERROR);
    }

    return false;
}
