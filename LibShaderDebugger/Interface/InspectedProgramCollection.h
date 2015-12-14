/*******************************************************************************\
*																				*
*			Class    : InspectedProgramCollection					            *
*			Purpose  : Collection of program inspector inspect context pairs    *
*			File     : InspectedProgramCollection.h							    *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __INSPECTED_PROGRAM_COLLECTION_H__
#define __INSPECTED_PROGRAM_COLLECTION_H__


#include "SmartPointerDeclarations.h"


// Forward declarations
CLASS_FORWARD_DECLARATIONS(InspectedProgram)


// Standard headers
#include <string>
#include <map>


// Library headers
#include "InspectedProgram.h"


class InspectedProgramCollection
{
public:
                                // Destructor
                                ~InspectedProgramCollection();

                                // Find program inspector combination
    SpInspectedProgram          Find(const std::string& inId) const;

                                // Add program inspector combination
    bool                        Add(SpInspectedProgram& inInspectedProgram);

                                // Delete program inspector combination by id
    bool                        Delete(const std::string& inId);

private:
    typedef std::map<std::string, SpInspectedProgram> tInspectedProgramMap;
    typedef std::pair<tInspectedProgramMap::iterator, bool> tInsertResult;
    tInspectedProgramMap        mInspectedProgramMap;
};


#endif // __INSPECTED_PROGRAM_COLLECTION_H__
