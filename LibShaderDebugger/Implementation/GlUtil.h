/*******************************************************************************\
*                                                                               *
*           Class   : GlUtil                                                    *
*           Purpose : Utility functions to simplify some gl manipulations       *
*           File    : GlUtil.h                                                  *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __GL_UTIL_H__
#define __GL_UTIL_H__


// ANGLE headers
#include <GLES2/gl2.h>


// Standard headers
#include <string>


// Append shader program info log to string
void
AppendProgramInfo(GLuint inHprg, std::string& ioText);


#endif // __GL_UTIL_H__
