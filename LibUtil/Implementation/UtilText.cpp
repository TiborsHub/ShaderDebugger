/*******************************************************************************\
*                                                                               *
*           Utility : UtilText                                                  *
*           Purpose : TExt related utility functions                            *
*           File    : UtilText.cpp                                              *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "UtilText.h"


// Standard headers
#include <assert.h>
#include <limits>


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


// Return offset in text from line nr and column index
// (Zero based)
size_t
GetOffsetFromLineColumn(
    const std::string& inText,
    size_t             inLineNr,
    size_t             inColumn)
{
    size_t line_nr(1);
    size_t col_ix(1);
    auto txt_len(inText.size());
    for (size_t ch_ix(0); ch_ix < txt_len; ++ch_ix)
    {
        char ch(inText[ch_ix]);
        if (ch == '\n')
        {
            ++line_nr;
            col_ix = 1;
        }
        else if (ch != '\r')
        {
            ++col_ix;
        }
        if (line_nr == inLineNr)
        {
            if (col_ix >= inColumn)
            {
                return ch_ix;
            }
        }
        else if (line_nr > inLineNr)
        {
            // Previous line was not long enough
            return ch_ix - 1;
        }
    }

    return std::numeric_limits<size_t>::max();
}
