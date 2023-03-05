
#include "HtmlHelp.h"
#include <wx/dir.h>
#include <wx/filename.h>

extern wxString blitzpath;

wxDEFINE_EVENT(OPEN_EVENT, wxCommandEvent);

HtmlHelp::HtmlHelp( wxWindow *parent,wxWindowID id ):wxPanel( parent,id ){
  browser = wxWebView::New(this, wxID_ANY);
  browser->EnableContextMenu( false );

  browser->Bind( wxEVT_WEBVIEW_NAVIGATING,&HtmlHelp::OnNavigating,this,wxID_ANY );

  GoHome();

  wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
  sizer->Add( browser,1,wxEXPAND,0 );
  SetSizer( sizer );
}

void HtmlHelp::OnNavigating( wxWebViewEvent& event ){
  wxString dirPath = event.GetURL();
  if( dirPath.StartsWith("file://") ) dirPath=dirPath.substr( 7,std::string::npos );

  wxDir dir( dirPath );
  if ( dir.IsOpened() ){
    event.StopPropagation();

    wxCommandEvent event( OPEN_EVENT,GetId() );
    event.SetEventObject( this );
    event.SetString( dirPath );
    ProcessWindowEvent( event );
  }
}

void HtmlHelp::GoHome(){
  wxFileName index( blitzpath+"/help/index.html" );
  index.MakeAbsolute();

  browser->ClearHistory();
  browser->LoadURL( "file://"+index.GetFullPath() );
}

bool HtmlHelp::GoBack(){
  browser->GoBack();
  return browser->CanGoBack();
}

bool HtmlHelp::GoForward(){
  browser->GoForward();
  return browser->CanGoForward();
}

bool HtmlHelp::CanGoBack(){
  return browser->CanGoBack();
}

bool HtmlHelp::CanGoForward(){
  return browser->CanGoForward();
}
