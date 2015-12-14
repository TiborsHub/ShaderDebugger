/*******************************************************************************\
*																				*
*			Class    : MousePosEvent  									        *
*			Purpose  : Event to send the mouse position inside a control to     *
*                      other parts of the gui                                   *
*			File     : MousePosEvent.cpp										    *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "MousePosEvent.h"


// Define event
wxDEFINE_EVENT(EVT_TYPE_CUST_MOUSE_POS, MousePosEvent);


// Clone object
// virtual
wxEvent*
MousePosEvent::Clone() const
{
    return new MousePosEvent(*this);
}
