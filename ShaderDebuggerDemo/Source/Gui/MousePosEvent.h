/*******************************************************************************\
*																				*
*			Class    : MousePosEvent  									        *
*			Purpose  : Event to send the mouse position inside a control to     *
*                      other parts of the gui                                   *
*			File     : MousePosEvent.h										    *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


#pragma once


#ifndef __MOUSE_POS_EVENT_H__
#define __MOUSE_POS_EVENT_H__


// wxWidgets headers
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// https://wiki.wxwidgets.org/Custom_Events

// Forward declaration
class MousePosEvent;


// Declare event
wxDECLARE_EVENT(EVT_TYPE_CUST_MOUSE_POS, MousePosEvent);


class MousePosEvent: public wxCommandEvent
{
public:
                                // Constructor
	                            MousePosEvent(wxEventType inCmdType, int inId, const wxPoint& inPosition);

                                	// Clone object
	virtual wxEvent*            Clone() const override;

                                // Return mouse position
	wxPoint                     GetPosition() const;

                                // Set mouse position
	void                        SetPosition(const wxPoint& inPosition);

private:
	wxPoint                     mPosition;
};


// Event handling
typedef void (wxEvtHandler::*tFnMousePosEvent)(MousePosEvent&);
#define MOUSE_POS_EVENT_HANDLER(inFunc) wxEVENT_HANDLER_CAST(tFnMousePosEvent, inFunc)


// Event table entry
#define EVT_CUST_MOUSE_POS(inId, inFunc) \
 	wx__DECLARE_EVT1(EVT_TYPE_CUST_MOUSE_POS, inId, MOUSE_POS_EVENT_HANDLER(inFunc))


// Inline definitions

// Constructor
inline
MousePosEvent::MousePosEvent(wxEventType inCmdType, int inId, const wxPoint& inPosition) :
    wxCommandEvent (inCmdType, inId),
    mPosition      (inPosition)
{

}


// Return mouse position
inline
wxPoint
MousePosEvent::GetPosition() const
{
    return mPosition;
}


// Set mouse position
inline
void
MousePosEvent::SetPosition(const wxPoint& inPosition)
{
    mPosition = inPosition;
}


#endif // __MOUSE_POS_EVENT_H__
