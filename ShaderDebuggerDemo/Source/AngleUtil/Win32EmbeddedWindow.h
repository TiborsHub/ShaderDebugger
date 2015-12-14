/*******************************************************************************\
*																				*
*			Class   : Win32EmbeddedWindow					                    *
*			Purpose : Renderer for WebGL programs                               *
*			File    : Win32EmbeddedWindow.h							            *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


// Windows headers
#include <Windows.h>


// ANGLE headers
#include <OSWindow.h>


#ifndef __WIN32_EMBEDDED_WINDOW_H__
#define __WIN32_EMBEDDED_WINDOW_H__


class Win32EmbeddedWindow : public OSWindow
{
public:
                                // Constructor
                                Win32EmbeddedWindow(HWND inHandle);

                                // Destructor
                                ~Win32EmbeddedWindow() override;

                                // Life time management
    virtual bool                initialize(const std::string &name, size_t width, size_t height) override;
    virtual void                destroy() override;

                                // Update dimensions from handle
    void                        updateDimensions();

                                // Return configuration property
    virtual EGLNativeWindowType getNativeWindow() const override;
    virtual EGLNativeDisplayType getNativeDisplay() const override;

                                // Drive message loop
    virtual void                messageLoop() override;

                                // Mouse handling
    virtual void                setMousePosition(int x, int y) override;
    virtual bool                setPosition(int x, int y) override;

                                // Sizing
    virtual bool                resize(int width, int height) override;

                                // Visibility
    virtual void                setVisible(bool isVisible) override;

                                // Testing
    virtual void                signalTestEvent() override;



private:
    HWND                        mHandleWin;
    HDC                         mHandleDC;
};


#endif // __WIN32_EMBEDDED_WINDOW_H__
