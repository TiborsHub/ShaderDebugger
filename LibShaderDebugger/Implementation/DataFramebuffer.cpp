/*******************************************************************************\
*																				*
*			Class   : DataFramebuffer	                                        *
*			Purpose : Framebuffer object which renders to a floating point      *
*                     texture to capture the nummeric output of a shader        *
*			File    : DataFramebuffer.cpp							            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "DataFramebuffer.h"


// Library headers
#include "Texture2D.h"
#include "PixelData.h"


// Standard headers
#include <assert.h>


// Constructor
DataFramebuffer::DataFramebuffer() :
    mPrevFbHandle (GL_ZERO),
    mData         (std::make_shared<PixelData2dF4>())

{
    glGenFramebuffers(1, &mHandle);
    assert(mHandle != GL_ZERO);
    assert(glGetError() == GL_NO_ERROR);

    if (mHandle != GL_ZERO)
    {

    }
}


// Destructor
// virtual
DataFramebuffer::~DataFramebuffer()
{
    if (mHandle != GL_ZERO)
    {
        glDeleteFramebuffers(1, &mHandle);
        assert(glGetError() == GL_NO_ERROR);
        mHandle = GL_ZERO;
    }
}


// Activate the framebuffer
void
DataFramebuffer::Bind(size_t inWidth, size_t inHeight)
{
    // Create texture if size of viewport has changed
    if (mTexture.get() == nullptr || mTexture->GetWidth() != inWidth || mTexture->GetHeight() != inHeight)
    {
        mData->SetDimensions(inWidth, inHeight);
        mTexture.reset(new Texture2D(
            GL_FLOAT,
            GL_RGBA,
            GL_CLAMP_TO_EDGE,
            GL_NEAREST,
            inWidth,
            inHeight,
            mData->GetMemoryBuffer()));
    }

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&mPrevFbHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
    assert(glGetError() == GL_NO_ERROR);

    // The texture is part of the framebuffer state so only need to be set if the texture is not yet bound
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        mTexture->GetHandle(),
        0);

    assert(glGetError() == GL_NO_ERROR);

    // Test framebuffer completion
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}


// Read the rendered data
SpConstPixelData2dF4
DataFramebuffer::ReadData()
{
    // Set pack parameter
    GLint prev_pack_val;
    glGetIntegerv(GL_PACK_ALIGNMENT, &prev_pack_val);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadPixels(
        0,
        0,
        mData->GetWidth(),
        mData->GetHeight(),
        GL_RGBA,
        GL_FLOAT,
        mData->GetMemoryBuffer());

    assert(glGetError() == GL_NO_ERROR);

    // Restore pack alignment
    glPixelStorei(GL_PACK_ALIGNMENT, prev_pack_val);

    return mData;
}


// Deactive the framebuffer
void
DataFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mPrevFbHandle);
    assert(glGetError() == GL_NO_ERROR);
}
