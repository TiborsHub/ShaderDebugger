/*******************************************************************************\
*                                                                               *
*           Utility : Log                                                       *
*           Purpose : Write text to logging facility                            *
*           File    : Log.h                                                     *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __LOG_H__
#define __LOG_H__


// Standard headers
#include <string>


enum LogInfoType
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};


// Write a line of text to logging facility
void
Log(LogInfoType inLogType, const std::string& inText);


#endif // __LOG_H__
