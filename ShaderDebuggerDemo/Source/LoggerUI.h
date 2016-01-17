/*******************************************************************************\
*                                                                               *
*           Utility : Logging facility which logs to control                    *
*           Purpose : Exposes interface to logging functionality                *
*           File    : LoggerUI.h                                                *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __LOGGER_UI_H__
#define __LOGGER_UI_H__


// Forward declarations
class wxRichTextCtrl;


// Library headers
#include "LoggerI.h"


class LoggerUI : public LoggerI
{
public:
                                // Constructor
                                LoggerUI(wxRichTextCtrl* inDebugCtrl);

                                // Create singleton
    static bool                 sCreateInstance(wxRichTextCtrl* inDebugCtrl);

                                // Create singleton
    static void                 sDestroyInstance();

    virtual void                Write(LogInfoType inType, const std::string& inText) override;

private:
    wxRichTextCtrl*             mDebugCtrl;
};


#endif // __LOGGER_UI_H__
