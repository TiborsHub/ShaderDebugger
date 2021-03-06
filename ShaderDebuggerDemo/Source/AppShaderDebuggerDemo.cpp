/*******************************************************************************\
*                                                                               *
*           Utility  : AppShaderDebuggerDemo                                    *
*           Purpose  : Creates the shader debugger demo application             *
*           File     : AppShaderDebuggerDemo.h                                  *
*           Author   : Tibor den Ouden                                          *
*                                                                               *
\*******************************************************************************/


// Project headers
#include "ShaderDebuggerDemo.h"


// wxWidgets headers
#include <wx/persist.h>
#include <wx/persist/toplevel.h>


// ANGLE headers
#include "../src/libANGLE/Display.h"
#include "../src/libGLESv2/global_state.h"


// The application
wxIMPLEMENT_APP_NO_MAIN(ShaderDebuggerDemo);


int
WINAPI
WinMain(HINSTANCE inInstanceH, HINSTANCE inPrevInstanceH, PSTR inCmdLine, int inCmdShow)
{
    // No debug support in release builds
    wxDISABLE_DEBUG_SUPPORT();

    int return_value(wxEntry(inInstanceH, inPrevInstanceH, inCmdLine, inCmdShow));

    // Fixes memory leak in wxWidgets
    wxPersistenceManager::sDestroy();
    wxConfigBase* config(wxConfigBase::Get(false)); // false : Do not create
    delete(config);

    // Fixes memory leaks in ANGLE
    eglFinalize();
    gl::DeInitGlobalContext();

    return return_value;
}
