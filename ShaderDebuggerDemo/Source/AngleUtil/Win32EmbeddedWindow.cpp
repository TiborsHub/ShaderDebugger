/*******************************************************************************\
*                                                                               *
*           Class   : Win32EmbeddedWindow                                       *
*           Purpose : Renderer for WebGL programs                               *
*           File    : Win32EmbeddedWindow.cpp                                   *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "Win32EmbeddedWindow.h"


// Standard headers
#include <assert.h>


// Constructor
Win32EmbeddedWindow::Win32EmbeddedWindow(HWND inHandle) :
    mHandleWin (inHandle),
    mHandleDC  (nullptr)
{
    updateDimensions();
}

// Destructor
Win32EmbeddedWindow::~Win32EmbeddedWindow()
{

}


// virtual
bool
Win32EmbeddedWindow::initialize(const std::string &inName, size_t inWidth, size_t inHeight)
{
    (void)inName;
    (void)inWidth;
    (void)inHeight;

    mHandleDC = GetDC(mHandleWin);
    if (mHandleDC == nullptr)
    {
        destroy();
        return false;
    }

    return true;
}


// virtual
void
Win32EmbeddedWindow::destroy()
{
    if (mHandleDC != nullptr)
    {
        ReleaseDC(mHandleWin, mHandleDC);
        mHandleDC = nullptr;
    }

    if (mHandleWin != nullptr)
    {
        DestroyWindow(mHandleWin);
        mHandleWin = nullptr;
    }
}


// Update dimensions from handle
void
Win32EmbeddedWindow::updateDimensions()
{
    RECT dim;
    if (GetClientRect(mHandleWin, &dim))
    {
        mWidth  = dim.right;
        mHeight = dim.bottom;
    }
    else
    {
        assert(false);
    }
}


// virtual
EGLNativeWindowType
Win32EmbeddedWindow::getNativeWindow() const
{
    return mHandleWin;
}


// virtual
EGLNativeDisplayType
Win32EmbeddedWindow::getNativeDisplay() const
{
    return mHandleDC;
}


// virtual
void
Win32EmbeddedWindow::messageLoop()
{
    assert(false);
}


// virtual
void
Win32EmbeddedWindow::setMousePosition(int inX, int inY)
{
    (void)inX;
    (void)inY;

    assert(false);
}


// virtual
bool
Win32EmbeddedWindow::setPosition(int inX, int inY)
{
    (void)inX;
    (void)inY;

    return false;
}


// virtual
bool
Win32EmbeddedWindow::resize(int inWidth, int inHeight)
{
    (void)inWidth;
    (void)inHeight;

    assert(false);

    return false;
}


// virtual
void
Win32EmbeddedWindow::setVisible(bool inSetVisible)
{
    (void)inSetVisible;

    assert(false);
}


// virtual
void
Win32EmbeddedWindow::signalTestEvent()
{
    assert(false);
}
