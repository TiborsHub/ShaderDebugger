/*******************************************************************************\
*																				*
*			Class    : EglRenderWindow								            *
*			Purpose  : Gui window on which the EGL system renders               *
*			File     : EglRenderWindow.h										    *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __EGL_RENDER_WINDOW_H__
#define __EGL_RENDER_WINDOW_H__


#include <wx/wx.h>
#include <wx/sizer.h>


class EglRenderWindow : public wxWindow
{
public:
                                // Constructor
                                EglRenderWindow(wxWindow* inParent);

                                // Destructor
                                ~EglRenderWindow();

                                // Return viewport dimensions
                                // Return width and height for now
    void                        GetViewport(int& outWidth, int& outHeight);

private:
                                // Handle paint event
    void                        OnPaint(wxPaintEvent& inEvent);

                                // Window resized
    void                        OnResize(wxSizeEvent& inEvent);

                                // Handle mouse move event
    void                        OnMouseMove(wxMouseEvent& inEvent);

                                // Handle mouse down event
    void                        OnMouseDown(wxMouseEvent& inEvent);

                                // Render contents
    void                        Render();

    wxDECLARE_EVENT_TABLE();
};


#endif // __EGL_RENDER_WINDOW_H__
