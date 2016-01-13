/*******************************************************************************\
*                                                                               *
*           Utility : UtilText                                                  *
*           Purpose : TExt related utility functions                            *
*           File    : UtilText.h                                                *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __UTIL_TEXT_H__
#define __UTIL_TEXT_H__


// Standard headers
#include <string>


// Find corresponding line number and column for an offset
// (One based)
void
FindLineColumnForOffset(
    const std::string& inText,
    size_t             inOffset,
    size_t&            outLineNr,
    size_t&            outColumn);


// Return offset in text from line nr and column index
// (One based)
size_t
GetOffsetFromLineColumn(
    const std::string& inText,
    size_t             inLineNr,
    size_t             inColumn);


#endif // __UTIL_TEXT_H__
