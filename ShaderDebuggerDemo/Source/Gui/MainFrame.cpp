/*******************************************************************************\
*                                                                               *
*           Class    : MainFrame                                                *
*           Purpose  : Main frame of shader debugger application                *
*           File     : MainFrame.cpp                                            *
*           Author   : Tibor den Ouden                                          *
*                                                                               *
\*******************************************************************************/


// Translation unit header
#include "MainFrame.h"


// Project headers
#include "EventId.h"
#include "CreateProgramInspectorInstance.h"
#include "InspectedProgram.h"
#include "ProgramInspectorI.h"
#include "InspectContextI.h"
#include "InspectResult.h"
#include "../AngleUtil/Win32EmbeddedWindow.h"
#include "EglRenderWindow.h"
#include "MousePosEvent.h"
#include "PixelData.h"
#include "../TestCases/TestCaseFactorySingleton.h"
#include "../TestCases/TestCaseFactory.h"


// wxWidget headers
#include <wx/richtext/richtextctrl.h>
#include <wx/splitter.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>


// ANGLE headers
#include <EGLWindow.h>


// Standard headers
#include <string>
#include <fstream>
#include <assert.h>


namespace
{

// Shader type names
wxString shader_types[] =
{
    wxT("Vertex"),
    wxT("Fragment"),
};

// WebGL version names
wxString webgl_versions[] =
{
    wxT("1"),
    wxT("2"),
};

} // namespace


// Event table
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_FILE_OPEN,              MainFrame::OnFileOpen)

    EVT_MENU_RANGE(ID_TEST_SHADER_FIRST, ID_TEST_SHADER_LAST, MainFrame::OnTestShader)

    EVT_RADIOBOX(ID_SHADER_TYPE,        MainFrame::OnShaderType)
    EVT_BUTTON(ID_RUN,                  MainFrame::OnRun)

    EVT_LEFT_UP(                        MainFrame::OnLeftSourceClick)

    EVT_MENU(wxID_EXIT,                 MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT,                MainFrame::OnAbout)

    EVT_CUST_MOUSE_POS(ID_VIEWPORT_POS, MainFrame::OnViewportPos)

wxEND_EVENT_TABLE()


// Constructor
MainFrame::MainFrame(
    const wxString& inTitle,
    wxPoint         inPos,
    wxSize          inSize
) :
    wxFrame            (nullptr, wxID_ANY, inTitle, inPos, inSize)
{
    // Menu file
    wxMenu *menu_file = new wxMenu;
/*
    menu_file->Append(
        ID_FILE_OPEN,
        "&Open...\tCtrl-O",
        "Open shader source");

    menu_file->AppendSeparator();
*/
    menu_file->Append(wxID_EXIT);

    // Menu test shaders
    assert(gTestCaseFactorySingleton->GetTestCaseCount() <= MAX_TEST_CASE_COUNT);

    wxMenu *menu_test_shaders = new wxMenu;
    int test_ix(0);
    for (auto test_it(gTestCaseFactorySingleton->Begin()); test_it != gTestCaseFactorySingleton->End(); ++test_it, ++test_ix)
    {
        menu_test_shaders->Append(ID_TEST_SHADER_FIRST + test_ix, test_it->first);
    }

    // Menu help
    wxMenu *menu_help = new wxMenu;
    menu_help->Append(wxID_ABOUT);

    // Menu bar
    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&File");
    menu_bar->Append(menu_test_shaders, "Test shaders");
    menu_bar->Append(menu_help, "&Help");
    SetMenuBar(menu_bar);

    // Main frame layout

    // ----------------------------------------------------------------------------
    // |                        |                          |                      |
    // |   Shader source        |                          |                      |
    // |                        |     Graphical output     |    Debug output      |
    // |                        |                          |                      |
    // |                        |                          |                      |
    // |                        |                          |                      |
    // |                        |--------------------------|                      |
    // |                        | Mouse pos in gl  window  |                      |
    // |                        |--------------------------|                      |
    // |                        |                          |                      |
    // |                        |      Inspect output      |                      |
    // |------------------------|      (Numeric)           |                      |
    // |                        |                          |                      |
    // |   Settings             |                          |                      |
    // |                        |                          |                      |
    // ----------------------------------------------------------------------------

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(main_sizer);

    wxSplitterWindow* left_splitter  = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);
    main_sizer->Add(left_splitter, 1, wxEXPAND);

    wxPanel* left_panel = new wxPanel(left_splitter);
    wxSplitterWindow* right_splitter = new wxSplitterWindow(left_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);

    wxPanel* mid_panel  = new wxPanel(right_splitter);
    wxPanel* right_panel  = new wxPanel(right_splitter);

    int width(GetClientSize().GetWidth() / 3);
    left_splitter->SplitVertically(left_panel, right_splitter, width);
    right_splitter->SplitVertically(mid_panel, right_panel, width);


    // Left panel contents
    mSourceCtrl = new wxRichTextCtrl(
        left_panel,
        wxID_ANY,
        wxEmptyString,
        wxDefaultPosition,
        wxDefaultSize,
        wxVSCROLL | wxHSCROLL);

    wxFont source_font(10, wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL);
    mSourceCtrl->SetFont(source_font);

    mSourceCtrl->AppendText(L"Shader source, select a test from the menu");

    mSourceCtrl->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnLeftSourceClick, this);

    wxGridBagSizer *left_sizer = new wxGridBagSizer(10, 10);

    left_sizer->Add(
        mSourceCtrl,
        wxGBPosition(0, 0),
        wxGBSpan(1, 3),
        wxEXPAND);

    // Shader type selection
    mShaderType = new wxRadioBox(
        left_panel,
        ID_SHADER_TYPE,
        wxT("&Shader type"),
        wxPoint(10, 10), wxDefaultSize,
        WXSIZEOF(shader_types), shader_types,
        1,
        wxRA_SPECIFY_COLS);

        // Set focus of fragment shader
        mShaderType->SetSelection(1);

        // Disable vertex shader for now
        mShaderType->Enable(0, false);

    left_sizer->Add(
        mShaderType,
        wxGBPosition(1, 0),
        wxGBSpan(2, 1),
        wxSTRETCH_NOT | wxLEFT);

    // WebGL version
    mWebGlVersion = new wxRadioBox(
        left_panel,
        ID_WEBGL_VERSION,
        wxT("&WebGL version"),
        wxPoint(10, 10), wxDefaultSize,
        WXSIZEOF(webgl_versions), webgl_versions,
        1,
        wxRA_SPECIFY_COLS);

        // Disable WebGL 2
        mWebGlVersion->Enable(1, false);

    left_sizer->Add(
        mWebGlVersion,
        wxGBPosition(1, 1),
        wxGBSpan(2, 1),
        wxSTRETCH_NOT | wxLEFT);

    // Source CODE control should get extra space
    left_sizer->AddGrowableRow(0);

    // Right most column should get extra space
    left_sizer->AddGrowableCol(2);

    left_panel->SetSizerAndFit(left_sizer);


    // Mid panel contents
    wxSplitterWindow* mid_splitter = new wxSplitterWindow(mid_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH | wxSP_LIVE_UPDATE);

    wxBoxSizer* mid_sizer = new wxBoxSizer(wxVERTICAL);
    mid_sizer->Add(mid_splitter, 1, wxEXPAND);
    mid_panel->SetSizer(mid_sizer);

    wxPanel* egl_panel  = new wxPanel(mid_splitter);
    wxBoxSizer* egl_sizer = new wxBoxSizer(wxVERTICAL);
    egl_panel->SetSizer(egl_sizer);

    // The window in which the gl will render
    mEglRenderWindow.reset(new EglRenderWindow(egl_panel));
    egl_sizer->Add(mEglRenderWindow.get(), 1, wxEXPAND);

    // Position of mouse in egl window
    mRenderWindowPos = new wxStaticText(egl_panel, wxID_ANY, "");
    egl_sizer->Add(mRenderWindowPos, 0, wxSTRETCH_NOT);

    // Inspect information
    mInspectCtrl = new wxRichTextCtrl(
        mid_splitter,
        wxID_ANY,
        wxEmptyString,
        wxDefaultPosition,
        wxDefaultSize,
        wxVSCROLL | wxHSCROLL);

    wxFont inspect_font(10, wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL);
    mInspectCtrl->SetFont(inspect_font);
    mInspectCtrl->AppendText(L"Inspect information, click on symbol in shader source and move mouse in egl window");

    int height = GetClientSize().GetHeight() * 0.5;
    mid_splitter->SplitHorizontally(egl_panel, mInspectCtrl, height);


    // Right panel contents

    wxBoxSizer* right_sizer = new wxBoxSizer(wxVERTICAL);
    right_panel->SetSizer(right_sizer);

    // Debug window
    mDebugCtrl = new wxRichTextCtrl(
        right_panel,
        wxID_ANY,
        wxEmptyString,
        wxDefaultPosition,
        wxDefaultSize,
        wxVSCROLL | wxHSCROLL);
    right_sizer->Add(mDebugCtrl, 1, wxEXPAND);

    wxFont debug_font(10, wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL);
    mDebugCtrl->SetFont(debug_font);
    mDebugCtrl->AppendText(L"Debug information, parse tree and modified shader are listed here");


    // Status bar
    CreateStatusBar();
    SetStatusText("Status text");


    // GLES Initialization
    mRenderWindow.reset(new Win32EmbeddedWindow(mEglRenderWindow->GetHWND()));
    bool render_result = mRenderWindow->initialize("", 0, 0);
    EGLint gles_major_version(2);
    EGLint gles_minor_version(0);
    EGLint requested_renderer(EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE);
    mEglWindow.reset(new EGLWindow(gles_major_version, gles_minor_version, EGLPlatformParameters(requested_renderer)));
    if (!mEglWindow->initializeGL(mRenderWindow.get()))
    {
        SetStatusText("EGL : Failed to initialize the render window");
    }
    else
    {
        mRenderWindow->updateDimensions();
        glViewport(0, 0, mRenderWindow->getWidth(), mRenderWindow->getHeight());
        SetStatusText("EGL : Successful initialized");
    }
}


// Returns the selected shader type
size_t
MainFrame::GetSelectedShaderIndex() const
{
    return mShaderType->GetSelection();
}


// Set shader source
void
MainFrame::SetShaderSource(const std::string& inSource)
{
    mSourceCtrl->Clear();
    mSourceCtrl->AppendText(inSource);
}


// Set debug information
void
MainFrame::SetDebugInfo(const std::string& inText)
{
    mDebugCtrl->Clear();
    mDebugCtrl->AppendText(inText);
}


// Return shader source
std::string
MainFrame::GetEditorShaderSource()
{
    std::string code;
    int line_count(mSourceCtrl->GetNumberOfLines());
    for (int l_ix = 0; l_ix < line_count; ++l_ix)
    {
        code.append(mSourceCtrl->GetLineText(l_ix).ToStdString());
    }

    return code;
}


void
MainFrame::OnExit(wxCommandEvent& inEvent)
{
    (void)inEvent;

    Close(true);
}


void
MainFrame::OnAbout(wxCommandEvent& inEvent)
{
    (void)inEvent;

    wxMessageBox(
        "Developed by Tibor den Ouden",
        "About Shader debugger",
        wxOK | wxICON_INFORMATION );
}


void
MainFrame::OnFileOpen(wxCommandEvent& inEvent)
{
    (void)inEvent;

    wxFileDialog openFileDialog(
        this,
        _("Open shader source"),
        "", "",
        "Shader files (*.*)|*.*",
        wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        // Action canceled
        return;
    }

    mSourceCtrl->Clear();

    std::wstring path(openFileDialog.GetPath().ToStdWstring());
    std::wifstream file(path);
    while (file.good())
    {
        std::wstring line;
        std::getline(file, line);
        if (!line.empty())
        {
            line.append(L"\n");
            mSourceCtrl->AppendText(line);
        }
    }
}


// Set test shader
void
MainFrame::OnTestShader(wxCommandEvent& inEvent)
{
    assert(inEvent.GetEventType() == wxEVT_MENU);
    wxMenu* menu( static_cast<wxMenu*>(inEvent.GetEventObject()));
    wxMenuItem* menu_item(menu->FindItem(inEvent.GetId()));
    std::string shader_test_id(menu_item->GetItemLabelText());

    mInspectData.reset();
    mInspectCtrl->Clear();
    mDebugCtrl->Clear();
    SpInspectContextI inspect_context(gTestCaseFactorySingleton->CreateTestCase(shader_test_id));
    if (inspect_context.get() != nullptr)
    {
        std::string msg;
        bool result(inspect_context->Initialize(msg));
        if (!result)
        {
            SetStatusText("Initialization of inspect context failed" + msg);
        }
        else
        {
            SpProgramInspectorI inspector(CreateProgramInspectorInstance(inspect_context, GetSelectedShaderIndex()));

            mInspectedProgram = std::make_shared<InspectedProgram>(
                shader_test_id,
                inspect_context,
                inspector);
        }
    }

    wxCommandEvent dummy_event;
    OnShaderType(dummy_event);
    OnRun(dummy_event);
}


// Change of displayed shader type
void
MainFrame::OnShaderType(wxCommandEvent& inEvent)
{
    (void)inEvent;

    std::string shader_source;
    if (mInspectedProgram.get() != nullptr)
    {
        size_t shader_ix(GetSelectedShaderIndex());
        shader_source = mInspectedProgram->GetInspectContext()->GetShaderSource(shader_ix);
    }

    SetShaderSource(shader_source);
}


// Run shader
void
MainFrame::OnRun(wxCommandEvent& inEvent)
{
    (void)inEvent;

    if (mInspectedProgram.get() != nullptr)
    {
        mRenderWindow->updateDimensions();

        // Set the viewport
        glViewport(0, 0, mRenderWindow->getWidth(), mRenderWindow->getHeight());

        mInspectedProgram->GetInspectContext()->UseProgram();
        mInspectedProgram->GetInspectContext()->RunTargetProgram();

        mEglWindow->swap();
    }
}


// User selected a token in shader source
void
MainFrame::OnLeftSourceClick(wxMouseEvent& inEvent)
{
    wxPoint pos(inEvent.GetPosition());
    long source_pos_ix;
    wxTextCtrlHitTestResult hit_result(mSourceCtrl->HitTest(pos, &source_pos_ix));
    if (hit_result == wxTE_HT_ON_TEXT)
    {
        // Selected a valid character in the source
        InspectResult inspect_result;
        mInspectedProgram->GetInspector()->Inspect(source_pos_ix, inspect_result);

        if (!inspect_result.mInfoText.empty())
        {
            SetDebugInfo(inspect_result.mInfoText);
            mInspectData = inspect_result.mInspectData;
        }
    }
}


// Update viewport information
void
MainFrame::UpdateViewportInfo(const wxPoint& inViewportMousePos)
{
    if (mInspectedProgram.get() != nullptr)
    {
        // Current view port size
        int vp_width;
        int vp_height;
        mEglRenderWindow->GetViewport(vp_width, vp_height);

        int frag_x(inViewportMousePos.x);
        int frag_y(vp_height - inViewportMousePos.y);

        // Check computation for border pixels
        float dev_x(2.0f * float(frag_x) / float(vp_width)  - 1.0f);
        float dev_y(2.0f * float(frag_y) / float(vp_height) - 1.0f);
        wxString info;
        info.Printf(wxT("Frag coord : %d, %d  dev coord : %f, %f"),
            frag_x,
            frag_y,
            dev_x,
            dev_y);

        mRenderWindowPos->SetLabel(info);

        // Retrieve data from inspect buffer
        if (   mInspectData.get() != nullptr
            && frag_x >= 0
            && frag_x <  int(mInspectData->GetWidth())
            && frag_y >= 0
            && frag_y <  int(mInspectData->GetHeight()))
        {
            Pixel pixel(mInspectData->GetPixel(frag_x, frag_y));
            wxString info;
            info.Printf(wxT("R : %f\nG : %f\nB : %f\nA : %f"),
                pixel[0],
                pixel[1],
                pixel[2],
                pixel[3]);

            mInspectCtrl->Clear();
            mInspectCtrl->AppendText(info);
        }
    }
}


// Mouse position in egl render window
void
MainFrame::OnViewportPos(MousePosEvent& inEvent)
{
    UpdateViewportInfo(inEvent.GetPosition());
}


