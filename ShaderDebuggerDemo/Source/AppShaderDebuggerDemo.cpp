/*******************************************************************************\
*																				*
*			Utility  : ShaderDebuggerDemoApp						                *
*			Purpose  : Creates the shader debugger demo application             *
*			File     : ShaderDebuggerDemoApp.h							        *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Project headers
#include "ShaderDebuggerDemo.h"


// The application
wxIMPLEMENT_APP_NO_MAIN(ShaderDebuggerDemo);


int
WINAPI
WinMain(HINSTANCE inInstanceH, HINSTANCE inPrevInstanceH, PSTR inCmdLine, int inCmdShow)
{
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    wxDISABLE_DEBUG_SUPPORT();

    int return_value(wxEntry(inInstanceH, inPrevInstanceH, inCmdLine, inCmdShow));

#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif

    return return_value;
}
