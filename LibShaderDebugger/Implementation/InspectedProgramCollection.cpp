/*******************************************************************************\
*																				*
*			Class    : InspectedProgramCollection					            *
*			Purpose  : Collection of program inspector inspect context pairs    *
*			File     : InspectedProgramCollection.cpp						    *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "InspectedProgramCollection.h"


// Standard headers
#include <assert.h>


// Destructor
InspectedProgramCollection::~InspectedProgramCollection()
{
    assert(mInspectedProgramMap.empty());
}


// Find program inspector combination
SpInspectedProgram
InspectedProgramCollection::Find(const std::string& inId) const
{
    tInspectedProgramMap::const_iterator it(mInspectedProgramMap.find(inId));

    return it->second;
}


// Add program inspector combination
bool
InspectedProgramCollection::Add(SpInspectedProgram& inInspectedProgram)
{
    tInsertResult result(mInspectedProgramMap.insert(
        tInspectedProgramMap::value_type(inInspectedProgram->GetId(), inInspectedProgram)));

    return result.second;
}


// Delete program inspector combination by id
bool
InspectedProgramCollection::Delete(const std::string& inId)
{
    return (mInspectedProgramMap.erase(inId) == 1);
}

