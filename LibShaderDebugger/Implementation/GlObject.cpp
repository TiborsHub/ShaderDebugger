/*******************************************************************************\
*																				*
*			Class   : GlObject	                                                *
*			Purpose : Base class for all objects which live in/on the OpenGL ES *
*                     context                                                   *
*			File    : GlObject.h							                        *
*			Author  : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "GlObject.h"


// Standard headers
#include <assert.h>


// Constructor
GlObject::GlObject() :
    mHandle(GL_ZERO)
{

}


// Destructor
// virtual
GlObject::~GlObject()
{
    assert(mHandle == GL_ZERO);
}
