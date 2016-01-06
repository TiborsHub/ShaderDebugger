/*******************************************************************************\
*                                                                               *
*           Class   : DataFramebuffer                                           *
*           Purpose : Framebuffer object which renders to a floating point      *
*                     texture to capture the nummeric output of a shader        *
*           File    : DataFramebuffer.h                                         *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __DATA_FRAMEBUFFER_H__
#define __DATA_FRAMEBUFFER_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(Texture2D);
CLASS_FORWARD_DECLARATIONS(PixelData2dF4);


// Library headers
#include "GlObject.h"


// ANGLE headers
#include "GLES2/gl2.h"


// Standard headers
#include <vector>
#include <cstdint>


class DataFramebuffer : public GlObject
{
public:
                                // Constructor
                                DataFramebuffer();

                                // Destructor
    virtual                     ~DataFramebuffer() override;

                                // Initialize object
    bool                        Initialize();

                                // Make the framebuffer active
    void                        Bind(size_t inWidth, size_t inHeight);

                                // Read the rendered data
    SpConstPixelData2dF4        ReadData();

                                // Deactivate the framebuffer
    void                        Unbind();

                                // Return data
    SpConstPixelData2dF4        GetData() const;

private:
    GLuint                      mPrevFbHandle;
    UpTexture2D                 mTexture;
    SpPixelData2dF4             mData;
};


// Inline definitions

// Return data
inline
SpConstPixelData2dF4
DataFramebuffer::GetData() const
{
    return mData;
}


#endif // __DATA_FRAMEBUFFER_H__
