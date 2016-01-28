/*******************************************************************************\
*                                                                               *
*           Class    : SourceLocation                                           *
*           Purpose  : Location of character interval in source                 *
*           File     : SourceLocation.h                                         *
*           Author   : Tibor den Ouden                                          *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __SOURCE_LOCATION_H__
#define __SOURCE_LOCATION_H__


// Indicates section of source code
// Interval of source characters (boundaries inclusive)
class SourceLocation
{
public:
                                // Constructor
                                SourceLocation() :
                                    mLineNrFirst (-1),
                                    mColIxFirst  (-1),
                                    mLineNrLast  (-1),
                                    mColIxLast   (-1)
                                {}

                                // Constructor
                                SourceLocation(
                                    int inLineNrFirst,
                                    int inColIxFirst,
                                    int inLineNrLast,
                                    int inColIxLast);

    int                         mLineNrFirst;
    int                         mColIxFirst;
    int                         mLineNrLast;
    int                         mColIxLast;
};


// Inline definitions

// Constructor
inline
SourceLocation::SourceLocation(
    int inLineNrFirst,
    int inColIxFirst,
    int inLineNrLast,
    int inColIxLast)
:
    mLineNrFirst (inLineNrFirst),
    mColIxFirst  (inColIxFirst),
    mLineNrLast  (inLineNrLast),
    mColIxLast   (inColIxLast)
{

}


#endif // __SOURCE_LOCATION_H__
