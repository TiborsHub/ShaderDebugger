/*******************************************************************************\
*                                                                               *
*           Utility : Logging facility which logs to control                    *
*           Purpose : Exposes interface to logging functionality                *
*           File    : LoggerUI.cpp                                              *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "LoggerUI.h"


// wxWidget headers
#include <wx/richtext/richtextctrl.h>


// Standard headers
#include <cassert>


// Constructor
LoggerUI::LoggerUI(wxRichTextCtrl* inDebugCtrl) :
    mDebugCtrl(inDebugCtrl)
{

}


// Create singleton
// static
bool
LoggerUI::sCreateInstance(wxRichTextCtrl* inDebugCtrl)
{
    assert(gLoggerSingleton == nullptr);

    gLoggerSingleton = new LoggerUI(inDebugCtrl);

    return (gLoggerSingleton != nullptr);
}


// Destroy singleton
// static
void
LoggerUI::sDestroyInstance()
{
    assert(gLoggerSingleton != nullptr);

    delete gLoggerSingleton;
    gLoggerSingleton = nullptr;
}


// virtual
void
LoggerUI::Write(LogInfoType inType, const std::string& inText)
{
    mDebugCtrl->AppendText(inText);
}
