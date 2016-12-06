
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#if !wxUSE_WEBVIEW_WEBKIT && !wxUSE_WEBVIEW_IE
#error "You need wxWebView to build this."
#endif

#include <wx/notebook.h>
#include <wx/filedlg.h>

#include "HtmlHelp.h"
#include "FileView.h"

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
};
class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void AddFile( wxString &path );

    void UpdateToolbar( int pageIndex );
private:
    wxToolBar *toolbar;
    HtmlHelp *help;
    wxNotebook *nb;

    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnNew( wxCommandEvent& event );
    void OnOpen( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event );
    void OnHome( wxCommandEvent& event );
    void OnBack( wxCommandEvent& event );
    void OnForward( wxCommandEvent& event );

    void OnPageChanged( wxBookCtrlEvent& event );

    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Hello = 1,
    ID_NEW,
    ID_OPEN,
    ID_SAVE,
    ID_SAVE_AS,
    ID_SAVE_ALL,
    ID_CLOSE,
    ID_CLOSE_ALL,
    ID_CUT,
    ID_COPY,
    ID_PASTE,
    ID_FIND,
    ID_RUN,
    ID_HOME,
    ID_BACK,
    ID_FORWARD
};
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_Hello,   MainFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(ID_NEW,     MainFrame::OnNew)
    EVT_MENU(ID_OPEN,    MainFrame::OnOpen)
    EVT_MENU(ID_CLOSE,   MainFrame::OnClose)
    EVT_MENU(ID_HOME,    MainFrame::OnHome)
    EVT_MENU(ID_BACK,    MainFrame::OnBack)
    EVT_MENU(ID_FORWARD, MainFrame::OnForward)

    EVT_COMMAND (wxID_ANY, OPEN_EVENT, MainFrame::OnOpen)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
  MainFrame *frame = new MainFrame( "Blitz3D",wxPoint( 50,50 ),wxSize( 800,600 ) );
  frame->Show( true );
  return true;
}
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE|wxNO_FULL_REPAINT_ON_RESIZE)
{
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_NEW, "&New...\tCtrl-N", "Help string shown in status bar for this menu item");
  menuFile->Append(ID_OPEN, "&Open...\tCtrl-O", "Help string shown in status bar for this menu item");
  menuFile->AppendSeparator();
  menuFile->Append(ID_CLOSE, "&Close\tCtrl-W", "Help string shown in status bar for this menu item");
  menuFile->Append(ID_CLOSE_ALL, "&Close All", "Help string shown in status bar for this menu item");
  menuFile->AppendSeparator();
  menuFile->Append(ID_SAVE, "&Save\tCtrl-W", "Help string shown in status bar for this menu item");
  menuFile->Append(ID_SAVE_AS, "&Save As...", "Help string shown in status bar for this menu item");
  menuFile->Append(ID_SAVE_ALL, "&Save All", "Help string shown in status bar for this menu item");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu *menuEdit = new wxMenu;
  menuEdit->Append(wxID_ANY, "Cut");
  menuEdit->Append(wxID_ANY, "Copy");
  menuEdit->Append(wxID_ANY, "Paste");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_ANY, "Select All");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_ANY, "Find...");
  menuEdit->Append(wxID_ANY, "Find Next");
  menuEdit->Append(wxID_ANY, "Replace...");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_ANY, "Show toolbars?");

  wxMenu *menuProgram = new wxMenu;
  menuProgram->Append(wxID_ANY, "Run program");
  menuProgram->Append(wxID_ANY, "Run program again");
  menuProgram->Append(wxID_ANY, "Check for errors");
  menuProgram->Append(wxID_ANY, "Create executable...");
  menuProgram->AppendSeparator();
  menuProgram->Append(wxID_ANY, "Program Command line...");
  menuProgram->Append(wxID_ANY, "Debug enabled?");

  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(ID_HOME, "Home");
  menuHelp->Append(ID_BACK, "Back");
  menuHelp->Append(ID_FORWARD, "Forward");
  menuHelp->Append(ID_FORWARD, "Quick command help");
  menuHelp->Append(wxID_ABOUT, "About Blitz3D!");

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append( menuFile, "&File" );
  menuBar->Append( menuEdit, "&Edit" );
  menuBar->Append( menuProgram, "&Program" );
  menuBar->Append( menuHelp, "&Help" );
  SetMenuBar( menuBar );

  wxImage::AddHandler( new wxPNGHandler );

  wxImage icons(wxT(BLITZPATH "/cfg/ide_toolbar.bmp"), wxBITMAP_TYPE_BMP);
  icons.SetMask( true );
  icons.SetMaskColour( 0xc0,0xc0,0xc0 );

  wxImage icon_new     = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*0,0));
  wxImage icon_open    = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*1,0));
  wxImage icon_save    = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*2,0));
  wxImage icon_close   = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*3,0));
  wxImage icon_cut     = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*4,0));
  wxImage icon_copy    = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*5,0));
  wxImage icon_paste   = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*6,0));
  wxImage icon_find    = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*7,0));
  wxImage icon_run     = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*8,0));
  wxImage icon_home    = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*9,0));
  wxImage icon_back    = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*10,0));
  wxImage icon_forward = wxImage(icons).Resize(wxSize(16,16),wxPoint(-16*11,0));

  toolbar = CreateToolBar();
  toolbar->AddTool( ID_NEW,wxT("New"),icon_new );
  toolbar->AddTool( ID_OPEN,wxT("Open"),icon_open );
  toolbar->AddTool( ID_SAVE,wxT("Save"),icon_save );
  toolbar->AddTool( ID_CLOSE,wxT("Close"),icon_close );
  toolbar->AddTool( ID_CUT,wxT("Cut"),icon_cut );
  toolbar->AddTool( ID_COPY,wxT("Copy"),icon_copy );
  toolbar->AddTool( ID_PASTE,wxT("Paste"),icon_paste );
  toolbar->AddTool( ID_FIND,wxT("Find"),icon_find );
  toolbar->AddTool( ID_RUN,wxT("Run"),icon_run );
  toolbar->AddTool( ID_HOME,wxT("Home"),icon_home );
  toolbar->AddTool( ID_BACK,wxT("Back"),icon_back );
  toolbar->AddTool( ID_FORWARD,wxT("Forward"),icon_forward );
  toolbar->Realize();

  CreateStatusBar();
  // SetStatusText( "" );

  nb = new wxNotebook( this,wxID_ANY );
  nb->Bind( wxEVT_NOTEBOOK_PAGE_CHANGED,&MainFrame::OnPageChanged,this,wxID_ANY );

  help = new HtmlHelp( nb,wxID_ANY );
  nb->AddPage( help,wxT("Help") );

  wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
  sizer->Add( nb,1,wxEXPAND,0 );

  SetSizer( sizer );

  UpdateToolbar( 0 );
}

void MainFrame::AddFile( wxString &path ){
  wxString basename;

  if ( path.length() == 0 ){
    basename = wxT("<untitled>");
  } else {
    basename = path.Mid( path.find_last_of("/")+1 );
  }

  FileView *view = new FileView( path,nb,wxID_ANY );
  nb->AddPage( view,basename );
}

void MainFrame::UpdateToolbar( int pageIndex ){
  toolbar->EnableTool( ID_SAVE,pageIndex>0 );
  toolbar->EnableTool( ID_CLOSE,pageIndex>0 );
  toolbar->EnableTool( ID_CUT,pageIndex>0 );
  toolbar->EnableTool( ID_COPY,pageIndex>0 );
  toolbar->EnableTool( ID_PASTE,pageIndex>0 );
  toolbar->EnableTool( ID_FIND,pageIndex>0 );
  toolbar->EnableTool( ID_RUN,pageIndex>0 );
  toolbar->EnableTool( ID_BACK,pageIndex==0 && help->CanGoBack() );
  toolbar->EnableTool( ID_FORWARD,pageIndex==0 && help->CanGoForward() );
}

void MainFrame::OnExit(wxCommandEvent& event){
    Close( true );
}
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
void MainFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::OnNew( wxCommandEvent& WXUNUSED(event) ){
  wxString path("");
  AddFile( path );
  nb->SetSelection( nb->GetPageCount()-1 );
}

void MainFrame::OnOpen( wxCommandEvent& event ){
  wxFileDialog openFileDialog(this, wxT("Open Blitz Basic file..."), event.GetString(), "", "Blitz Basic files (*.bb)|*.bb", wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE);

  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxArrayString paths;
  openFileDialog.GetPaths( paths );

  wxArrayString::iterator it;
  for( it=paths.begin();it<paths.end();it++ ){
    AddFile( *it );
  }
  nb->SetSelection( nb->GetPageCount()-1 );
}

void MainFrame::OnClose( wxCommandEvent& WXUNUSED(event) ){
  size_t page = nb->GetSelection();
  if ( page==0 ) return;

  nb->DeletePage( page );
  UpdateToolbar( page-1 );
}

void MainFrame::OnHome( wxCommandEvent& WXUNUSED(event) ){
  nb->SetSelection( 0 );
  help->GoHome();
}

void MainFrame::OnBack( wxCommandEvent& WXUNUSED(event) ){
  help->GoBack();
}

void MainFrame::OnForward( wxCommandEvent& WXUNUSED(event) ){
  help->GoForward();
}

void MainFrame::OnPageChanged( wxBookCtrlEvent& event ){
  UpdateToolbar( event.GetSelection() );
}
