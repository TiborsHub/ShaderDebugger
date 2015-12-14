/*******************************************************************************\
*																				*
*			Class    : ShaderDebuggerDemo								        *
*			Purpose  : Main class of shader debugger demo application           *
*			File     : ShaderDebuggerDemo.h										*
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __SHADER_DEBUGGER_DEMO_H__
#define __SHADER_DEBUGGER_DEMO_H__


// wxWidgets headers
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class ShaderDebuggerDemo : public wxApp
{
public:
                                // Constructor
                                ShaderDebuggerDemo();

                                // Destructor
                                ~ShaderDebuggerDemo();

                                // Initialization of application
    virtual bool                OnInit() override;

                                // Termination of application
    virtual int                 OnExit() override;
};


#endif // __SHADER_DEBUGGER_DEMO_H__
