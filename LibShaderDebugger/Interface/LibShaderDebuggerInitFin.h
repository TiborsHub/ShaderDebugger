/*******************************************************************************\
*																				*
*			Utility : LibShaderDebuggerInitFin					                *
*			Purpose : Utility functions to initialize and finalize the          *
*                     shader debugger library                                   *
*			File    : LibShaderDebuggerInitFin.h							        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __LIB_SHADER_DEBUGGER_INIT_FIN_H__
#define __LIB_SHADER_DEBUGGER_INIT_FIN_H__


// Initialize the shader debugger library
bool
LibShaderDebuggerInitialize();


// Finalize the shader debugger library
bool
LibShaderDebuggerFinalize();


#endif // __LIB_SHADER_DEBUGGER_INIT_FIN_H__
