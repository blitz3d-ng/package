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
	signerId->Bind( wxEVT_CHOICE,&PreferencesDialog::OnSignIdChange,this,wxID_ANY );
	signerId->Append( "" );

	wxStaticText *teamLabel=new wxStaticText( p2,wxID_ANY,"Team ID" );
	wxChoice *teamId=new wxChoice( p2,wxID_ANY );
	teamId->Bind( wxEVT_CHOICE,&PreferencesDialog::OnTeamIdChange,this,wxID_ANY );
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
				teams.push_back( ProvisioningProfile( id ) );
				teamId->Append( id );
			}
			cont=dir.GetNext( &filename );
		}

		int idx=teamId->FindString( prefs->teamId );
		if( idx>-1 ){
			teamId->SetSelection( idx );
		}
	}

	//

	int signerIdx=-1;
	wxArrayString results;
	wxExecute( "security find-identity -v -p codesigning",results );
	for( wxString &line:results ){
		if( line.EndsWith( "\"" ) ){
			wxString label,id;
			line=line.BeforeLast('"');
			label=line.AfterLast('"');
			line=line.BeforeLast('"');
			id=line.AfterLast(')').Trim(true).Trim(false);
			signers.push_back( SigningIdentity( label,id ) );
			signerId->Append( label );

			if( id==prefs->signId ){
				signerIdx=signers.size();
			}
		}
	}
	std::cout<<signerIdx<<std::endl;
	if( signerIdx>-1 ){
		signerId->SetSelection( signerIdx );
	}

	nb->AddPage( p2,wxT("Code Signing") );
#endif

	wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
	sizer->AddSpacer( 5 );
	sizer->Add( nb,1,wxEXPAND,0 );
	sizer->AddSpacer( 5 );
	SetSizer( sizer );
}

void PreferencesDialog::OnSignIdChange( wxCommandEvent& event ){
	prefs->signId=signers[event.GetInt()-1].id;
	prefs->Save();
}

void PreferencesDialog::OnTeamIdChange( wxCommandEvent& event ){
	prefs->teamId=teams[event.GetInt()-1].id;
	prefs->Save();
}
