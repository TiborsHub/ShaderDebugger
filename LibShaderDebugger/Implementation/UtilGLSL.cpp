/*******************************************************************************\
*                                                                               *
*           Utility : UtilGLSL                                                  *
*           Purpose : Utility functions for GLSL shader sources                 *
*           File    : UtilGLSL.h                                                *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "UtilGLSL.h"


// Translation unit local definitions
namespace
{

// Separators in
char sSeparators[] = ".+-/*%<>[](){}^|&~=!:;,? \t\v\f\r\n";

}


// Returns true if character is a glsl separator
bool
IsSeparatorGLSL(char inChar)
{
    return (strchr(sSeparators, inChar) != nullptr);
}


// Returns position of first character of glsl symbol
size_t
FindFirstCharOfSymbolGLSL(const std::string& inSource, size_t inSymbolIx)
{
    size_t first_ch_ix(inSymbolIx);
    while (first_ch_ix > 0 && !IsSeparatorGLSL(inSource[first_ch_ix - 1]))
    {
        --first_ch_ix;
    }

    return first_ch_ix;
}


// Returns position of last character of glsl symbol
size_t
FindLastCharOfSymbolGLSL(const std::string& inSource, size_t inSymbolIx)
{
   size_t last_ch_ix(inSymbolIx);
    while (last_ch_ix < inSource.size() - 1 && !IsSeparatorGLSL(inSource[last_ch_ix + 1]))
    {
        ++last_ch_ix;
    }

    return last_ch_ix;
}
