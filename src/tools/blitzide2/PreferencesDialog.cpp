#include "PreferencesDialog.h"

wxBEGIN_EVENT_TABLE(PreferencesDialog, wxDialog)
wxEND_EVENT_TABLE()

PreferencesDialog::PreferencesDialog ( wxWindow * parent )
	: wxDialog( parent,wxID_ANY,"Preferences",wxDefaultPosition,wxSize( 400,500 ),wxCAPTION|wxCLOSE_BOX|wxSTAY_ON_TOP )
{
	wxSize pos=(parent->GetSize()-GetSize())*0.5;
	SetPosition( parent->GetPosition()+wxPoint( pos.x,pos.y ) );

	nb = new wxNotebook( this,wxID_ANY );

	wxPanel *p1=new wxPanel( nb,wxID_ANY );
	nb->AddPage( p1,wxT("Editor") );
#ifdef BB_MACOS
	wxPanel *p2=new wxPanel( nb,wxID_ANY );

	wxTextCtrl *teamId=new wxTextCtrl( p2,wxID_ANY,"",wxDefaultPosition,wxDefaultSize,0, wxDefaultValidator, wxTextCtrlNameStr);

	wxListBox *signerId=new wxListBox( p2,wxID_ANY );
	signerId->Append("kevin");
	signerId->Append("primm");

	wxBoxSizer *sizer2 = new wxBoxSizer( wxVERTICAL );
	sizer2->Add( teamId,1,wxEXPAND|wxLEFT|wxRIGHT,0 );
	sizer2->Add( signerId,1,wxEXPAND|wxLEFT|wxRIGHT,0 );
	p2->SetSizer( sizer2 );

	nb->AddPage( p2,wxT("Code Signing") );
#endif

	wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
	sizer->AddSpacer( 5 );
	sizer->Add( nb,1,wxEXPAND,0 );
	sizer->AddSpacer( 5 );
	SetSizer( sizer );
}
