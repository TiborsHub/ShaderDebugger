/*******************************************************************************\
*																				*
*			Class    : EglRenderWindow								            *
*			Purpose  : Gui window on which the EGL system renders               *
*			File     : EglRenderWindow.cpp										*
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "EglRenderWindow.h"


// Project headers
#include "EventId.h"
#include "MousePosEvent.h"


// ANGLE headers
#include "GLES2/gl2.h"



// Constructor
EglRenderWindow::EglRenderWindow(wxWindow* inParent) :
    wxWindow(inParent, wxID_ANY)
{

}


// Destructor
EglRenderWindow::~EglRenderWindow()
{

}


BEGIN_EVENT_TABLE(EglRenderWindow, wxWindow)
/*
    EVT_LEFT_UP(wxCustomButton::mouseReleased)
    EVT_RIGHT_DOWN(wxCustomButton::rightClick)
    EVT_LEAVE_WINDOW(wxCustomButton::mouseLeftWindow)
    EVT_KEY_DOWN(wxCustomButton::keyPressed)
    EVT_KEY_UP(wxCustomButton::keyReleased)
    EVT_MOUSEWHEEL(wxCustomButton::mouseWheelMoved)
 */
    EVT_MOTION    (EglRenderWindow::OnMouseMove)
    EVT_LEFT_DOWN (EglRenderWindow::OnMouseDown)
    EVT_PAINT     (EglRenderWindow::OnPaint)
    EVT_SIZE      (EglRenderWindow::OnResize)

END_EVENT_TABLE()


// Handle mouse move event
void
EglRenderWindow::OnMouseMove(wxMouseEvent& inEvent)
{
    MousePosEvent event(
        EVT_TYPE_CUST_MOUSE_POS,
        ID_VIEWPORT_POS,
        inEvent.GetPosition());

    ProcessEvent(event);
}


// Handle mouse down event
void
EglRenderWindow::OnMouseDown(wxMouseEvent& inEvent)
{
}


// Handle paint event
void
EglRenderWindow::OnPaint(wxPaintEvent& inEvent)
{
    (void)inEvent;

    Render();
}


// Render contents
void
EglRenderWindow::Render()
{
    wxCommandEvent event(wxEVT_BUTTON, ID_RUN);
    ProcessEvent(event);
}


// Window resized
void
EglRenderWindow::OnResize(wxSizeEvent& inEvent)
{
    Render();
}


// Return viewport dimensions
// Return width and height for now
void 
EglRenderWindow::GetViewport(int& outWidth, int& outHeight)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    outWidth  = viewport[2];
    outHeight = viewport[3];
}