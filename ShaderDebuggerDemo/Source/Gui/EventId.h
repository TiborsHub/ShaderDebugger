/*******************************************************************************\
*                                                                               *
*           enum     : EventId                                                  *
*           Purpose  : Collection of program inspector inspect context pairs    *
*           File     : EventId.h                                                *
*           Author   : Tibor den Ouden                                          *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __EVENT_ID_H__
#define __EVENT_ID_H__


static const int MAX_TEST_CASE_COUNT = 100;


// Menu ids
enum EventId
{
    ID_FILE_OPEN,

    ID_SHADER_TYPE,
    ID_WEBGL_VERSION,
    ID_PARSE,
    ID_RUN,
    ID_STEP,
    ID_RESET,

    ID_VIEWPORT_POS,

    // Test shader ids
    ID_TEST_SHADER_FIRST,
    ID_TEST_SHADER_LAST  = ID_TEST_SHADER_FIRST + MAX_TEST_CASE_COUNT,
};


#endif // __EVENT_ID_H__
