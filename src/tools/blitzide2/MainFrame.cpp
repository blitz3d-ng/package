#include "MainFrame.h"
#include "PreferencesDialog.h"
#include "dpi.h"

extern wxString blitzpath;

enum{
	ID_NEW = 1,
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
	ID_EXE,
	ID_LOCKBUILD,
	ID_UNLOCKBUILD,
	ID_HOME,
	ID_BACK,
	ID_FORWARD,

	ID_TARGET=500
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT,        MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT,       MainFrame::OnAbout)
	EVT_MENU(ID_NEW,           MainFrame::OnNew)
	EVT_MENU(ID_OPEN,          MainFrame::OnOpen)
	EVT_MENU(ID_SAVE,          MainFrame::OnSave)
	EVT_MENU(ID_CLOSE,         MainFrame::OnClose)
	EVT_MENU(wxID_PREFERENCES, MainFrame::OnPreferences)
	EVT_MENU(ID_RUN,           MainFrame::OnRun)
	EVT_MENU(ID_EXE,           MainFrame::OnExe)
	EVT_MENU(ID_LOCKBUILD,     MainFrame::OnLockFile)
	EVT_MENU(ID_UNLOCKBUILD,   MainFrame::OnUnlockFile)
	EVT_MENU(ID_HOME,          MainFrame::OnHome)
	EVT_MENU(ID_BACK,          MainFrame::OnBack)
	EVT_MENU(ID_FORWARD,       MainFrame::OnForward)

	EVT_COMMAND (wxID_ANY, OPEN_EVENT, MainFrame::OnOpen)

	EVT_COMMAND (wxID_ANY, BUILD_BEGIN,    MainFrame::OnBuildBegin)
	EVT_COMMAND (wxID_ANY, BUILD_PROGRESS, MainFrame::OnBuildProgress)
	EVT_COMMAND (wxID_ANY, BUILD_END,      MainFrame::OnBuildEnd)
	EVT_COMMAND (wxID_ANY, BUILD_KILL,     MainFrame::OnBuildKill)
wxEND_EVENT_TABLE()

MainFrame::MainFrame( const wxString& title )
	: wxFrame( NULL,wxID_ANY,title,wxDefaultPosition,wxDefaultSize,wxDEFAULT_FRAME_STYLE|wxNO_FULL_REPAINT_ON_RESIZE )
{
	SetIcon( wxIcon("IDI_ICON1") );

	prefs.Load( blitzpath );

  wxMenu *menuFile = new wxMenu;
  menuFile->Append( ID_NEW,"&New...\tCtrl-N" );
  menuFile->Append( ID_OPEN,"&Open...\tCtrl-O" );
  menuFile->AppendSeparator();
  menuFile->Append( ID_CLOSE,"&Close\tCtrl-W" );
  menuFile->Append( ID_CLOSE_ALL,"&Close All" );
  menuFile->AppendSeparator();
  menuFile->Append( ID_SAVE,"&Save\tCtrl-S" );
  menuFile->Append( ID_SAVE_AS,"&Save As...\tShift-Ctrl-S" );
  menuFile->Append( ID_SAVE_ALL,"&Save All" );
  menuFile->AppendSeparator();
	menuFile->Append( wxID_PREFERENCES,"&Preferences\tCtrl-," );
  menuFile->AppendSeparator();
  menuFile->Append( wxID_EXIT );

  wxMenu *menuEdit = new wxMenu;
  menuEdit->Append( wxID_ANY,"Cut" );
  menuEdit->Append( wxID_ANY,"Copy" );
  menuEdit->Append( wxID_ANY,"Paste" );
  menuEdit->AppendSeparator();
  menuEdit->Append( wxID_ANY,"Select All" );
  menuEdit->AppendSeparator();
  menuEdit->Append( wxID_ANY,"Find..." );
  menuEdit->Append( wxID_ANY,"Find Next" );
  menuEdit->Append( wxID_ANY,"Replace..." );
  menuEdit->AppendSeparator();
  menuEdit->Append( wxID_ANY,"Show toolbars?" );

  wxMenu *menuProgram = new wxMenu;
  menuProgram->Append( ID_RUN,"Run program\tCtrl-R" );
  menuProgram->Append( wxID_ANY,"Run program again" );
  menuProgram->Append( wxID_ANY,"Check for errors" );
  menuProgram->Append( ID_EXE,"Create executable..." );
	platformMenu=new wxMenu;
	menuProgram->Append( wxID_ANY,"Build for...",platformMenu );
  menuProgram->AppendSeparator();
	menuProgram->Append( ID_LOCKBUILD,"Lock build file\tCtrl-L" );
	menuProgram->Append( ID_UNLOCKBUILD,"Unlock build file\tShift-Ctrl-L" );
  menuProgram->AppendSeparator();
  menuProgram->Append( wxID_ANY,"Program Command line..." );
  menuProgram->Append( wxID_ANY,"Debug enabled?" );

  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append( ID_HOME,"Home" );
  menuHelp->Append( ID_BACK,"Back" );
  menuHelp->Append( ID_FORWARD,"Forward" );
  menuHelp->Append( ID_FORWARD,"Quick command help" );
  menuHelp->Append( wxID_ABOUT,"About Blitz3D!" );

  menuBar = new wxMenuBar;
  menuBar->Append( menuFile, "&File" );
  menuBar->Append( menuEdit, "&Edit" );
  menuBar->Append( menuProgram, "&Program" );
  menuBar->Append( menuHelp, "&Help" );
  SetMenuBar( menuBar );

	platformMenu->Bind( wxEVT_MENU,&MainFrame::OnTarget,this,wxID_ANY );
	platformMenu->Check( ID_TARGET,true );
	deviceIdx=0;

  wxImage::AddHandler( new wxPNGHandler );

  wxImage icons(blitzpath + "/cfg/ide_toolbar.bmp", wxBITMAP_TYPE_BMP);
  icons.SetMask( true );
  icons.SetMaskColour( 0xc0,0xc0,0xc0 );

  wxSize iconSize=wxSize( 16,16 );

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

#ifdef BB_MACOS
	toolbar = 0;
#else
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
#endif

  CreateStatusBar();
  // SetStatusText( "" );

  nb = new wxNotebook( this,wxID_ANY );
  nb->Bind( wxEVT_NOTEBOOK_PAGE_CHANGED,&MainFrame::OnPageChanged,this,wxID_ANY );

  help = new HtmlHelp( nb,wxID_ANY );
  nb->AddPage( help,wxT("Help") );

  wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
  sizer->AddSpacer( 5 );
  sizer->Add( nb,1,wxEXPAND,0 );

  SetSizer( sizer );

	buildFile=0;

	UpdateToolbar( 0 );
	UpdateMenu( 0 );
	EnumerateDevices();
}

void MainFrame::AddFile( wxString &path ){
	FileView *view = new FileView( path,nb,wxID_ANY );
	nb->AddPage( view,view->GetTitle() );
}

void MainFrame::AddFiles( wxArrayString &paths ){
	if( paths.size()==0 ){
		return;
	}

	wxArrayString::iterator it;
	for( it=paths.begin();it<paths.end();it++ ){
		AddFile( *it );
	}
	nb->SetSelection( nb->GetPageCount()-1 );
}

void MainFrame::UpdateToolbar( int pageIndex ){
	if( toolbar==0 ) return;

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

void MainFrame::UpdateMenu( int pageIndex ){
	if( pageIndex==0 ){
		menuBar->Enable( ID_RUN,false );
	}else{
		menuBar->Enable( ID_RUN,true );
	}
}

void MainFrame::UnlockFile(){
	if( buildFile==0 ) return;

	size_t page=nb->FindPage( buildFile );
	nb->SetPageText( page,buildFile->GetTitle() );
	buildFile=0;
}

void MainFrame::EnumerateDevices(){
#ifdef BB_MACOS
	wxArrayString output;
	wxExecute( "xcrun xctrace list devices",output,wxEXEC_HIDE_CONSOLE );
	bool device=false;
	for( const wxString &line:output ){
		if( line=="== Devices ==" ){
			device=true;
		}else if( line=="" ){
			device=false;
		}else if( !device ){
			continue;
		}else{
			if( line.Last()!=')' ){
				continue;
			}

			wxString buf=line.BeforeLast(')');

			Target dev;
			dev.id=buf.AfterLast('(').Trim(true).Trim(false);
			dev.name=line.BeforeLast('(').Trim(true).Trim(false);
			dev.emulator=false;

			// a little bit of a hack...
			if( devices.size()==0 ){
				dev.name="macOS";
				dev.platform="macos";
				dev.host=true;
			}else{
				dev.platform="ios";
				dev.host=false;
			}

			devices.push_back( dev );
		}
	}



	output.Clear();
	wxExecute( "xcrun simctl list devices",output,wxEXEC_HIDE_CONSOLE );

	device=false;
	for( const wxString &line:output ){
		if( line.StartsWith( "-- iOS" ) ){
			device=true;
		}else if( line.StartsWith( "-- " ) ){
			device=false;
		}else if( !device ){
			continue;
		}else{
			wxString buf=line.BeforeLast(')');

			Target dev;
			dev.running=buf.AfterLast('(')=="Booted";
			buf=buf.BeforeLast(')');
			dev.id=buf.AfterLast('(').Trim(true).Trim(false);
			dev.name=line.BeforeLast('(').BeforeLast('(').Trim(true).Trim(false);
			dev.host=false;
			dev.emulator=true;
			dev.platform="ios-sim";
			devices.push_back( dev );
		}
	}
#endif

	for( int i=0;i<platformMenu->GetMenuItemCount();i++ ){
		platformMenu->Remove( 0 );
	}

	wxMenuItem *devs=platformMenu->Append( wxID_ANY,"Device" );
	devs->Enable( false );

	for( size_t i=0;i<devices.size();i++ ){
		if( !devices[i].emulator ){
			platformMenu->AppendCheckItem( ID_TARGET+i,devices[i].name );
		}
	}

	platformMenu->AppendSeparator();

	wxMenuItem *sims=platformMenu->Append( wxID_ANY,"Simulator" );
	sims->Enable( false );

	for( size_t i=0;i<devices.size();i++ ){
		if( devices[i].emulator ){
			platformMenu->AppendCheckItem( ID_TARGET+i,devices[i].name );
		}
	}
}

void MainFrame::OnExit(wxCommandEvent& event){
  Close( true );
}

void MainFrame::OnAbout(wxCommandEvent& event){
  wxMessageBox( "...",
                "About Blitz3D \"NG\"", wxOK | wxICON_INFORMATION );
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
  AddFiles( paths );
}

void MainFrame::OnSave( wxCommandEvent& WXUNUSED(event) ){
  size_t page = nb->GetSelection();
  if ( page==0 ) return;

	FileView *file=dynamic_cast<FileView*>( nb->GetCurrentPage() );
	file->Save();
}

void MainFrame::OnClose( wxCommandEvent& WXUNUSED(event) ){
  size_t page = nb->GetSelection();
  if ( page==0 ) return;

  nb->DeletePage( page );
  UpdateToolbar( page-1 );
}

void MainFrame::OnPreferences( wxCommandEvent& WXUNUSED(event) ){
	PreferencesDialog *dlg=new PreferencesDialog( this,&prefs );
	dlg->ShowModal();
}

void MainFrame::OnRun( wxCommandEvent& WXUNUSED(event) ){
  size_t page = nb->GetSelection();
  if ( page==0 ) return;

	FileView *file=buildFile;
	if( !file ){
		file=dynamic_cast<FileView*>( nb->GetCurrentPage() );
	}

	file->Execute( devices[deviceIdx],&prefs );
}

void MainFrame::OnExe( wxCommandEvent& WXUNUSED(event) ){
  size_t page=nb->GetSelection();
  if ( page==0 ) return;

	FileView *file=dynamic_cast<FileView*>( nb->GetCurrentPage() );
	if( !file ){
		return;
	}

	wxFileDialog saveExeDialog( this,wxT("Create executable..."),"","","",wxFD_SAVE|wxFD_OVERWRITE_PROMPT );
	if( saveExeDialog.ShowModal()==wxID_CANCEL )
		return;

	wxString out=saveExeDialog.GetPath();
	file->Build( out );
}

void MainFrame::OnTarget( wxCommandEvent& event ){
	for( int i=0;i<platformMenu->GetMenuItemCount();i++ ){
		if( event.GetId()!=ID_TARGET+i ){
			platformMenu->Check( ID_TARGET+i,false );
		}
	}
	deviceIdx=event.GetId()-ID_TARGET;
}

void MainFrame::OnBuildBegin( wxCommandEvent& event ){
	buildDialog=new BuildDialog( this );
	buildDialog->ShowModal();
}

void MainFrame::OnBuildProgress( wxCommandEvent& event ){
	buildDialog->AddMessage( event.GetString() );
}

void MainFrame::OnBuildEnd( wxCommandEvent& event ){
	buildDialog->EndModal( 0 );
	Raise();
}

void MainFrame::OnBuildKill( wxCommandEvent& event ){
	size_t page=nb->GetSelection();
	if ( page==0 ) return;

	FileView *file=dynamic_cast<FileView*>( nb->GetCurrentPage() );
	file->Kill();
}

void MainFrame::OnLockFile( wxCommandEvent& WXUNUSED(event) ){
	size_t page=nb->GetSelection();
	if ( page==0 ) return;

	UnlockFile();

	buildFile=dynamic_cast<FileView*>( nb->GetCurrentPage() );
	nb->SetPageText( page,"build:"+buildFile->GetTitle() );
}

void MainFrame::OnUnlockFile( wxCommandEvent& WXUNUSED(event) ){
	if( buildFile==0 ) return;

	size_t page=nb->FindPage( buildFile );
	nb->SetPageText( page,buildFile->GetTitle() );
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
	UpdateMenu( event.GetSelection() );
}
