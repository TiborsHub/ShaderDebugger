/*******************************************************************************\
*                                                                               *
*           Utility : Interface to logging facility                             *
*           Purpose : Exposes interface to logging functionality                *
*           File    : LoggerI.h                                                 *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __LOGGER_I_H__
#define __LOGGER_I_H__


// Library headers
#include "Log.h"


class LoggerI
{
public:
                                // Destructor
    virtual                     ~LoggerI() {};

    virtual void                Write(LogInfoType inType, const std::string& inText) = 0;
};


extern
LoggerI* gLoggerSingleton;


#endif // __LOGGER_I_H__
