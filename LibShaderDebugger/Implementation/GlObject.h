/*******************************************************************************\
*                                                                               *
*           Class   : GlObject                                                  *
*           Purpose : Base class for all objects which live in/on the OpenGL ES *
*                     context                                                   *
*           File    : GlObject.h                                                *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __GL_OBJECT_H__
#define __GL_OBJECT_H__


// ANGLE headers
#include "GLES2/gl2.h"


class GlObject
{
public:
                                // Constructor
                                GlObject();

                                // Destructor
    virtual                     ~GlObject();

                                // Return handle
    GLuint                      GetHandle() const;

protected:
    GLuint                      mHandle;
};


// Inline definitions

// Return handle
inline
GLuint
GlObject::GetHandle() const
{
    return mHandle;
}


#endif // __GL_OBJECT_H__
