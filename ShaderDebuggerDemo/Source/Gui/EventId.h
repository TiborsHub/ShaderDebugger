/*******************************************************************************\
*																				*
*			enum     : EventId					                                *
*			Purpose  : Collection of program inspector inspect context pairs    *
*			File     : EventId.h							                        *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __EVENT_ID_H__
#define __EVENT_ID_H__


// Menu ids
enum EventId
{
    ID_FILE_OPEN,

    // Test shader ids
    ID_TEST_SINGLE_TRIANGLE,
    ID_TEST_CODE,
    ID_TEST_FB_READ,
    ID_TEST_READ_RED,
    ID_TEST_READ_GREEN,
    ID_TEST_READ_BLUE,
    ID_TEST_READ_ALPHA,
    ID_TEST_READ_DEV_COORDS,
    ID_TEST_FADE_RED_BOTTOM,
    ID_TEST_SHADER_FIRST = ID_TEST_SINGLE_TRIANGLE,
    ID_TEST_SHADER_LAST  = ID_TEST_FADE_RED_BOTTOM,

    ID_SHADER_TYPE,
    ID_WEBGL_VERSION,
    ID_PARSE,
    ID_RUN,
    ID_COMPILE,

    ID_VIEWPORT_POS,
};


#endif // __EVENT_ID_H__
