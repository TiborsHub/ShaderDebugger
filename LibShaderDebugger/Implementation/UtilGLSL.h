/*******************************************************************************\
*																				*
*			Utility : UtilGLSL					                                *
*			Purpose : Utility functions for GLSL shader sources                 *
*			File    : UtilGLSL.h							                        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __UTIL_GLSL_H__
#define __UTIL_GLSL_H__


// Standard headers
#include <string>


// Returns true if character is a glsl separator
bool
IsSeparatorGLSL(char inChar);


// Returns position of first character of glsl symbol
size_t
FindFirstCharOfSymbolGLSL(const std::string& inSource, size_t inSymbolIx);


// Returns position of last character of glsl symbol
size_t
FindLastCharOfSymbolGLSL(const std::string& inSource, size_t inSymbolIx);


#endif // __UTIL_GLSL_H__
