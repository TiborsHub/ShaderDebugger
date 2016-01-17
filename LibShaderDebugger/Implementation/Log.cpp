/*******************************************************************************\
*                                                                               *
*           Utility : Log                                                       *
*           Purpose : Write text to logging facility                            *
*           File    : Log.cpp                                                   *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "Log.h"


// Library headers
#include "LoggerI.h"


// The single instance
LoggerI*
gLoggerSingleton(nullptr);


// Write a line of text to logging facility
void
Log(LogInfoType inLogType, const std::string& inText)
{
    if (gLoggerSingleton != nullptr)
    {
        gLoggerSingleton->Write(inLogType, inText);
    }
}
