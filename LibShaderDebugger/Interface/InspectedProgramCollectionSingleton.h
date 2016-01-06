/*******************************************************************************\
*                                                                               *
*           Utility  : InspectedProgramCollectionSingleton                      *
*           Purpose  : The single inspected program collection instance         *
*           File     : InspectedProgramCollectionSingleton.h                    *
*           Author   : Tibor den Ouden                                          *
*                                                                               *
\*******************************************************************************/


#pragma once


#ifndef __INSPECTED_PROGRAM_COLLECTION_SINGLETON_H__
#define __INSPECTED_PROGRAM_COLLECTION_SINGLETON_H__


// Forward declaration
class InspectedProgramCollection;


// The single instance
extern
InspectedProgramCollection*
gInspectedProgramCollectionSingleton;


// Create singleton instance
bool
CreateInspectedProgramCollectionSingleton();


// Destroy singleton instance
void
DestroyInspectedProgramCollectionSingleton();


#endif // __INSPECTED_PROGRAM_COLLECTION_SINGLETON_H__
