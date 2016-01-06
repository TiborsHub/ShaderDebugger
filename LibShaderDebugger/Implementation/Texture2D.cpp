/*******************************************************************************\
*                                                                               *
*           Class   : Texture2D                                                 *
*           Purpose : Object which contains a 2 dimension array of color data   *
*           File    : Texture2D.cpp                                             *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "Texture2D.h"


// ANGLE headers
#include "GLES2/gl2.h"


// Standard headers
#include <assert.h>


// Constructor
Texture2D::Texture2D(
    GLenum   inPixelCompType,
    GLint    inPixelFormat,
    GLint    inWrapType,
    GLint    inFilterType,
    size_t   inWidth,
    size_t   inHeight,
    uint8_t* inData
) :
    mWidth  (inWidth),
    mHeight (inHeight)
{
    glGenTextures(1, &mHandle);
    assert(mHandle != GL_ZERO);

    if (mHandle != GL_ZERO)
    {
        // Store current active texture unit
        GLint prev_tex_unit;
        glGetIntegerv(GL_ACTIVE_TEXTURE, &prev_tex_unit);

        // Set texture unit
        glActiveTexture(GL_TEXTURE0);
        assert(glGetError() == GL_NO_ERROR);

        // Store current binding
        GLint prev_tex_2d;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_tex_2d);

        // Bind this texture
        glBindTexture(GL_TEXTURE_2D, mHandle);
        assert(glGetError() == GL_NO_ERROR);

        // Set the parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     inWrapType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     inWrapType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, inFilterType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, inFilterType);

        // Set texture data
        // No pixel format conversion
        glTexImage2D(
            GL_TEXTURE_2D,
            0,          // No mip mapping
            inPixelFormat,
            inWidth,
            inHeight,
            0,          // Border must be 0 always
            inPixelFormat,
            inPixelCompType,
            inData);

        assert(glGetError() == GL_NO_ERROR);

        // Restore previous 2d texture
        glBindTexture(GL_TEXTURE_2D, prev_tex_2d);

        // Restore active texture unit
        glActiveTexture(prev_tex_unit);
        assert(glGetError() == GL_NO_ERROR);
    }
}


// Destructor
// virtual
Texture2D::~Texture2D()
{
    if (mHandle != GL_ZERO)
    {
        glDeleteTextures(1, &mHandle);
        assert(glGetError() == GL_NO_ERROR);
        mHandle = GL_ZERO;
    }
}
