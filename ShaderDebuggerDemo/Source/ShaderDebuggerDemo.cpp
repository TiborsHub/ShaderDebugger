/*******************************************************************************\
*																				*
*			Class    : ShaderDebuggerDemo								        *
*			Purpose  : Main class of shader debugger demo application           *
*			File     : ShaderDebuggerDemo.cpp									*
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "ShaderDebuggerDemo.h"


// Check memory allocations
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#endif


// Project headers
#include "Gui/MainFrame.h"
#include "CreateProgramInspectorInstance.h"
#include "LibShaderDebuggerInitFin.h"
#include "InspectedProgramCollectionSingleton.h"


// wxWidgets headers
#include <wx/persist.h>
#include <wx/persist/toplevel.h>


// Constructor
ShaderDebuggerDemo::ShaderDebuggerDemo()
{

}


// Destructor
ShaderDebuggerDemo::~ShaderDebuggerDemo()
{

}


// Initialization of application
// virtual
bool
ShaderDebuggerDemo::OnInit()
{
    if (LibShaderDebuggerInitialize() && CreateInspectedProgramCollectionSingleton())
    {
        MainFrame *frame = new MainFrame(
            "Shader debugger",
            wxPoint(200, 100),
            wxSize(1000, 800));

        frame->SetName("MainFrame");
        if (!wxPersistenceManager::Get().RegisterAndRestore(frame))
        {

        }

        frame->Show(true);
        return true;
    }

    return false;
}


// Termination of application
// virtual
int
ShaderDebuggerDemo::OnExit()
{
    wxPersistenceManager::sDestroy();

    int exit_status(0);
    if (!LibShaderDebuggerFinalize())
    {
        // "Failed to finalize the compiler.";
        exit_status = 1;
    }

    DestroyInspectedProgramCollectionSingleton();

    return exit_status;
}
