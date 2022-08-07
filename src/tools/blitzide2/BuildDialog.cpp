#include "BuildDialog.h"

wxBEGIN_EVENT_TABLE(BuildDialog, wxDialog)
	EVT_BUTTON( wxID_ANY,BuildDialog::OnTerminate )
wxEND_EVENT_TABLE()

BuildDialog::BuildDialog ( wxWindow * parent )
	: wxDialog( parent,wxID_ANY,"Building...",wxDefaultPosition,wxDefaultSize,wxCAPTION|wxSTAY_ON_TOP )
{
	wxSize pos=(parent->GetSize()-GetSize())*0.5;
	SetPosition( parent->GetPosition()+wxPoint( pos.x,pos.y ) );

	wxSize size=GetClientSize();

	wxButton *term=new wxButton( this,wxID_OK,_("Terminate") );
	wxSize tsize=term->GetSize();
	term->SetPosition( wxPoint( size.GetWidth()-(tsize.GetWidth()+10),size.GetHeight()-(tsize.GetHeight()+10) ) );

	size=GetClientSize();
	size.DecBy( 0,tsize.GetHeight()+10+10 );
	output=new wxTextCtrl( this,-1,"",wxDefaultPosition,size,wxTE_MULTILINE|wxTE_READONLY );
}

void BuildDialog::AddMessage( const wxString &m ){
	output->AppendText( m+"\n" );
}

void BuildDialog::OnTerminate( wxCommandEvent & event ){
	wxPostEvent( GetParent(),wxCommandEvent( BUILD_KILL ) );
}
