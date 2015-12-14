/*******************************************************************************\
*																				*
*			Class   : GlUtil	                                                    *
*			Purpose : Utility functions to simplify some gl manipulations       *
*			File    : GlUtil.h							                        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "GlUtil.h"


// Test OpenGL and application character types
static_assert(sizeof(GLchar) == sizeof(std::string::value_type),
    "OpenGL character type has not the same size as std::string character type");


// Append shader program info log to string
void
AppendProgramInfo(GLuint inHprg, std::string& ioText)
{
    GLint info_log_length;
    glGetProgramiv(inHprg, GL_INFO_LOG_LENGTH, &info_log_length);
    size_t current_size(ioText.size());
    ioText.resize(current_size + info_log_length);
    glGetProgramInfoLog(
        inHprg,
        info_log_length,
        nullptr,
        const_cast<GLchar*>(ioText.data()) + current_size);
}
