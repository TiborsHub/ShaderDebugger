/*******************************************************************************\
*																				*
*			Class   : PixelData                                                 *
*			Purpose : Stores floating point color data in a 2 dimensional array *
*			File    : PixelData.h							                    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __PIXEL_DATA_H__
#define __PIXEL_DATA_H__


// Standard headers
#include <vector>
#include <cstdint>
#include <assert.h>


// Later : turn into template on size and component type
class Pixel
{
public:
                                // Constructor
                                Pixel();

    float                       Get(size_t inIx) const;
    float                       operator [] (size_t inIx) const;

                                // Return color components
                                // Need to verify if memory layout is correct
    float                       GetRed()   const { return mData[0]; };
    float                       GetGreen() const { return mData[1]; };
    float                       GetBlue()  const { return mData[2]; };
    float                       GetAlpha() const { return mData[3]; };

private:
    float                       mData[4];
};


// Later : Turn into template for pixel type
class PixelData2dF4
{
public:
                                // Constructor
                                PixelData2dF4();

                                // Set dimensions of buffer
    void                        SetDimensions(size_t inWidth, size_t inHeight);

                                // Return width
    size_t                      GetWidth() const;

                                // Return height
    size_t                      GetHeight() const;

                                // Return pixel
    Pixel                       GetPixel(size_t inX, size_t inY) const;

                                // Return buffer image
    const Pixel*                GetRawData() const;

                                // Raw memory buffer access
    uint8_t*                    GetMemoryBuffer();

private:
    size_t                      mWidth;
    size_t                      mHeight;
    std::vector<Pixel>          mData;
};


// Inline definitions

// Pixel functions

// Constructor
inline
Pixel::Pixel()
{
    mData[0] = 0.0;
    mData[1] = 0.0;
    mData[2] = 0.0;
    mData[3] = 0.0;
}


inline
float
Pixel::Get(size_t inIx) const
{
    assert(inIx < 4);
    return mData[inIx];
}


inline
float
Pixel::operator [] (size_t inIx) const
{
    return Get(inIx);
}


// PixelData functions

// Constructor
inline
PixelData2dF4::PixelData2dF4() :
    mWidth  (0),
    mHeight (0)
{

}


// Return width
inline
size_t
PixelData2dF4::GetWidth() const
{
    return mWidth;
}


// Return height
inline
size_t
PixelData2dF4::GetHeight() const
{
    return mHeight;
}


// Return pixel
inline
Pixel
PixelData2dF4::GetPixel(size_t inX, size_t inY) const
{
    assert(inX < mWidth);
    assert(inY < mHeight);

    return mData[inX + inY * mWidth];
}


// Return buffer image
inline
const Pixel*
PixelData2dF4::GetRawData() const
{
    return mData.data();
}


// Raw memory buffer access
inline
uint8_t*
PixelData2dF4::GetMemoryBuffer()
{
    return reinterpret_cast<uint8_t*>(mData.data());
}


#endif // __PIXEL_DATA_H__
