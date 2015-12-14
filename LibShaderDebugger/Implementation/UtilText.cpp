/*******************************************************************************\
*																				*
*			Utility : UtilText					                                *
*			Purpose : TExt related utility functions                            *
*			File    : UtilText.cpp							                    *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "UtilText.h"


// Standard headers
#include <assert.h>


// Find corresponding line number and column for an offset
// (One based)
void
FindLineColumnForOffset(
    const std::string& inText,
    size_t             inOffset,
    size_t&            outLineNr,
    size_t&            outColumn)
{
    assert(inOffset < inText.size());

    outLineNr = 1;
    outColumn = 1;
    for (size_t ch_ix(0); ch_ix < inOffset; ++ch_ix)
    {
        char ch(inText[ch_ix]);
        if (ch == '\n')
        {
            ++outLineNr;
            outColumn = 1;
        }
        else if (ch != '\r')
        {
            ++outColumn;
        }
    }
}
