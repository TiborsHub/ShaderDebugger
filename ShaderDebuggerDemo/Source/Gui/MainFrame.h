/*******************************************************************************\
*                                                                               *
*           Class    : MainFrame                                                *
*           Purpose  : Main frame of shader debugger application                *
*           File     : MainFrame.h                                              *
*           Author   : Tibor den Ouden                                          *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
class wxRichTextCtrl;
class wxPanel;
class wxRadioBox;
class wxStaticText;
class MousePosEvent;
CLASS_FORWARD_DECLARATIONS(ProgramInspectorI);
CLASS_FORWARD_DECLARATIONS(Win32EmbeddedWindow);
CLASS_FORWARD_DECLARATIONS(EGLWindow);
CLASS_FORWARD_DECLARATIONS(EglRenderWindow);
CLASS_FORWARD_DECLARATIONS(Program);
CLASS_FORWARD_DECLARATIONS(InspectContextI);
CLASS_FORWARD_DECLARATIONS(InspectedProgram);
CLASS_FORWARD_DECLARATIONS(PixelData2dF4);
CLASS_FORWARD_DECLARATIONS(wxRichTextRange);
CLASS_FORWARD_DECLARATIONS(wxTextAttr);
CLASS_FORWARD_DECLARATIONS(Debugger);


// wxWidgets headers
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class MainFrame: public wxFrame
{
public:
                                // Constructor
                                MainFrame(
                                    const wxString& inTitle,
                                    wxPoint         inPos,
                                    wxSize          inSize);

private:
                                // Returns the selected shader type
    size_t                      GetSelectedShaderIndex() const;

                                // UI event handlers
    void                        OnFileOpen(wxCommandEvent& event);
    void                        OnExit(wxCommandEvent& event);
    void                        OnAbout(wxCommandEvent& event);

                                // Change of displayed shader type
    void                        OnShaderType(wxCommandEvent& inEvent);

                                // Set test shader
    void                        OnTestShader(wxCommandEvent& inEvent);

                                // Run shader
    void                        OnRun(wxCommandEvent& event);

                                // Execute current statement and advance to next statement
    void                        OnStep(wxCommandEvent& event);

                                // Reset debug state of shader
    void                        OnReset(wxCommandEvent& event);

                                // User selected a token in shader source
    void                        OnLeftSourceClick(wxMouseEvent& inEvent);

                                // Mouse position in egl render window
    void                        OnViewportPos(MousePosEvent& inEvent);

                                // Return shader source in edit control
    std::string                 GetEditorShaderSource();

                                // Set shader source
    void                        SetShaderSource(const std::string& inSource);

                                // Set debug information
    void                        SetDebugInfo(const std::string& inText);

                                // Update viewport information
    void                        UpdateViewportInfo(const wxPoint& inViewportMousePos);

    std::string                 mShaderSource;
    wxPanel*                    mSourcePanel;
    wxRichTextCtrl*             mSourceCtrl;
    wxRadioBox*                 mShaderType;
    wxRadioBox*                 mWebGlVersion;
    wxRichTextCtrl*             mDebugCtrl;
    wxRichTextCtrl*             mInspectCtrl;
    wxStaticText*               mRenderWindowPos;
    UpWin32EmbeddedWindow       mRenderWindow;
    UpEGLWindow                 mEglWindow;
    UpEglRenderWindow           mEglRenderWindow;
    SpInspectedProgram          mInspectedProgram;
    SpConstPixelData2dF4        mInspectData;

    UpDebugger                  mDebugger;
    UpwxRichTextRange           mDebugStatementFocus;
    UpwxTextAttr                mDebugFocusStyle;

    wxDECLARE_EVENT_TABLE();
};


#endif // __MAIN_FRAME_H__
