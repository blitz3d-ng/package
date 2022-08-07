#include "PreferencesDialog.h"

wxBEGIN_EVENT_TABLE(PreferencesDialog, wxDialog)
wxEND_EVENT_TABLE()

PreferencesDialog::PreferencesDialog( wxWindow *parent,Preferences *prefs )
	: wxDialog( parent,wxID_ANY,"Preferences",wxDefaultPosition,wxSize( 400,500 ),wxCAPTION|wxCLOSE_BOX|wxSTAY_ON_TOP ),
		prefs(prefs)
{
	wxSize pos=(parent->GetSize()-GetSize())*0.5;
	SetPosition( parent->GetPosition()+wxPoint( pos.x,pos.y ) );

	nb = new wxNotebook( this,wxID_ANY );

	wxPanel *p1=new wxPanel( nb,wxID_ANY );
	nb->AddPage( p1,wxT("Editor") );
#ifdef BB_MACOS
	wxPanel *p2=new wxPanel( nb,wxID_ANY );

	wxStaticText *signerLabel=new wxStaticText( p2,wxID_ANY,"Certificate" );
	wxChoice *signerId=new wxChoice( p2,wxID_ANY );
	signerId->Append( "" );

	wxStaticText *teamLabel=new wxStaticText( p2,wxID_ANY,"Team ID" );
	wxChoice *teamId=new wxChoice( p2,wxID_ANY );
	teamId->Append( "" );

	wxBoxSizer *sizer2 = new wxBoxSizer( wxVERTICAL );
	sizer2->Add( signerLabel,0,wxEXPAND,0 );
	sizer2->Add( signerId,0,wxEXPAND,0 );
	sizer2->Add( teamLabel,0,wxEXPAND|wxTOP,10 );
	sizer2->Add( teamId,0,wxEXPAND,0 );
	p2->SetSizer( sizer2 );

	wxString userhome;
	wxGetEnv( "HOME",&userhome );

	wxDir dir( userhome+"/Library/MobileDevice/Provisioning Profiles" );
	if( dir.IsOpened() ){
		wxString filename;
		bool cont=dir.GetFirst( &filename,"*.mobileprovision" );
		while( cont ){
			wxArrayString results;
			wxExecute( "sh -c \"security cms -D -i \\\""+dir.GetName()+"/"+filename+"\\\" | plutil -extract TeamIdentifier.0 raw -\"",results,results );
			for( wxString &id:results ){
				teamId->Append( id );
			}
			cont=dir.GetNext( &filename );
		}
	}

	//

	wxArrayString results;
	wxExecute( "security find-identity -v -p codesigning",results );
	for( wxString &line:results ){
		if( line.EndsWith( "\"" ) ){
			line=line.BeforeLast('"');
			wxString label=line.AfterLast('"');
			line=line.BeforeLast('"');
			wxString id=line.AfterLast(')').Trim(true).Trim(false);
			signerId->Append( label );
		}
	}


	nb->AddPage( p2,wxT("Code Signing") );
#endif

	wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
	sizer->AddSpacer( 5 );
	sizer->Add( nb,1,wxEXPAND,0 );
	sizer->AddSpacer( 5 );
	SetSizer( sizer );
}
