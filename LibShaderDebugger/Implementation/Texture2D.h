/*******************************************************************************\
*                                                                               *
*           Class   : Texture2D                                                 *
*           Purpose : Object which contains a 2 dimension array of color data   *
*           File    : Texture2D.h                                               *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __TEXTURE_2D_H__
#define __TEXTURE_2D_H__


// Library headers
#include "GlObject.h"


// Standard headers
#include <cstdint>


class Texture2D : public GlObject
{
public:
                                // Constructor
                                Texture2D(
                                    GLenum   inPixelCompType,
                                    GLint    inPixelFormat,
                                    GLint    inWrapType,
                                    GLint    inFilterType,
                                    size_t   inWidth,
                                    size_t   inHeight,
                                    uint8_t* inData);

                                // Destructor
    virtual                     ~Texture2D() override;

                                // Return width;
    size_t                      GetWidth() const;

                                // return height;
    size_t                      GetHeight() const;

private:
    size_t                      mWidth;
    size_t                      mHeight;
};


// Inline definitions

// Return width;
inline
size_t
Texture2D::GetWidth() const
{
    return mWidth;
}


// return height;
inline
size_t
Texture2D::GetHeight() const
{
    return mHeight;
}


#endif // __TEXTURE_2D_H__
