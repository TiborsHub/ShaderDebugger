/*******************************************************************************\
*                                                                               *
*           Class   : PixelData                                                 *
*           Purpose : Stores floating point color data in a 2 dimensional array *
*           File    : PixelData.cpp                                             *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "PixelData.h"


// Databuffer2D functions

// Set dimensions of buffer
void
PixelData2dF4::SetDimensions(size_t inWidth, size_t inHeight)
{
    mWidth  = inWidth;
    mHeight = inHeight;
    mData.resize(inWidth * inHeight);
}
