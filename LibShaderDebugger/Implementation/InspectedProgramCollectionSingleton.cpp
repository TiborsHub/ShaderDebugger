/*******************************************************************************\
*																				*
*			Utility  : InspectedProgramCollectionSingleton			            *
*			Purpose  : The single inspected program collection instance         *
*			File     : InspectedProgramCollectionSingleton.cpp				    *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Transalation unit header
#include "InspectedProgramCollectionSingleton.h"


// Library headers
#include "InspectedProgramCollection.h"


// Standard headers
#include <assert.h>


// The single instance
InspectedProgramCollection*
gInspectedProgramCollectionSingleton(nullptr);


// Create singleton instance
bool
CreateInspectedProgramCollectionSingleton()
{
    assert(gInspectedProgramCollectionSingleton == nullptr);

    gInspectedProgramCollectionSingleton = new InspectedProgramCollection;
    return (gInspectedProgramCollectionSingleton  != nullptr);
}


// Destroy singleton instance
void
DestroyInspectedProgramCollectionSingleton()
{
    assert(gInspectedProgramCollectionSingleton != nullptr);

    delete gInspectedProgramCollectionSingleton;
    gInspectedProgramCollectionSingleton = nullptr;
}

