#include "FileView.h"
#include <wx/textfile.h>
#include <wx/process.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include "BlitzCC.h"

enum {
  MARGIN_LINE_NUMBERS
};

static wxString keywordsList;
static bool keywordsLoaded=false;

extern wxString blitzpath;

wxDEFINE_EVENT(FILE_VIEW_DIRTY_EVENT, wxCommandEvent);

void FileView::LoadKeywords(){
  if( !keywordsLoaded ){
    wxArrayString keywords;
    wxExecute( blitzpath + "/bin/blitzcc -k",keywords );

    keywordsList = "";
    wxArrayString::iterator it;
    for( it=keywords.begin();it<keywords.end();it++ ){
      keywordsList << " " << (*it).Lower();
    }

    keywordsLoaded = true;
  }
}

FileView::FileView( wxString &path,wxWindow *parent,wxWindowID id ):path(path),cc(0),wxPanel( parent,id ){
	dirty=path.length()>0;

	LoadKeywords();

	// Inconsolata, Monaco, Consolas, 'Courier New', Courier
	wxFont font;
#ifdef __WXMSW__
	font = wxFontInfo(12).FaceName("Consolas");
#else
#ifdef __WXOSX__
	font = wxFontInfo(14).FaceName("Monaco");
#else
	font = wxFontInfo(12).FaceName("Courier New");
#endif
#endif

	text=new wxStyledTextCtrl(this, wxID_ANY);
	text->CmdKeyAssign( 'A',wxSTC_KEYMOD_META,wxSTC_CMD_HOME );
	text->CmdKeyAssign( 'E',wxSTC_KEYMOD_META,wxSTC_CMD_LINEEND );

	if ( path.length()>0 ){
		Open( path );
	}

	wxColour rgb_bkgrnd=wxColour( 0x22,0x55,0x88 );
	wxColour rgb_string=wxColour( 0x00,0xff,0x66 );
	wxColour rgb_ident=wxColour( 0xff,0xff,0xff );
	wxColour rgb_keyword=wxColour( 0xaa,0xff,0xff );
	wxColour rgb_comment=wxColour( 0xff,0xee,0x00 );
	wxColour rgb_digit=wxColour( 0x33,0xff,0xdd );
	wxColour rgb_default=wxColour( 0xee,0xee,0xee );

	// text->StyleClearAll();

	text->StyleSetBackground( wxSTC_STYLE_DEFAULT,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_STYLE_DEFAULT,rgb_default );

	text->SetCaretForeground( wxColour( 0xff,0xff,0xff ) );
	text->SetCaretLineBackground( wxColour( 0x1e,0x4a,0x76 ) );
	text->SetCaretLineVisible( true );

	text->SetTabWidth( 4 );

	text->SetMarginWidth( MARGIN_LINE_NUMBERS,25 );
	text->SetMarginType( MARGIN_LINE_NUMBERS,wxSTC_MARGIN_NUMBER );

	text->SetLexer( wxSTC_LEX_BLITZBASIC );

	text->StyleSetFont( wxSTC_STYLE_DEFAULT,font );

	text->StyleSetForeground( wxSTC_STYLE_LINENUMBER, wxColour (0xee,0xee,0xee) );
	text->StyleSetBackground( wxSTC_STYLE_LINENUMBER, wxColour (0x1e,0x4a,0x76) );

	text->StyleSetBackground( wxSTC_B_DEFAULT,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_DEFAULT,rgb_default );
	text->StyleSetBackground( wxSTC_B_DATE,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_DATE,rgb_default );
	text->StyleSetBackground( wxSTC_B_OPERATOR,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_OPERATOR,rgb_default );
	text->StyleSetBackground( wxSTC_B_PREPROCESSOR,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_PREPROCESSOR,rgb_default );
	text->StyleSetBackground( wxSTC_B_IDENTIFIER,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_IDENTIFIER,rgb_ident );
	text->StyleSetBackground( wxSTC_B_STRING,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_STRING,rgb_string );
	text->StyleSetBackground( wxSTC_B_COMMENT,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_COMMENT,rgb_comment );
	text->StyleSetBackground( wxSTC_B_KEYWORD,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_KEYWORD,rgb_keyword );
	text->StyleSetBackground( wxSTC_B_NUMBER,rgb_bkgrnd );
	text->StyleSetForeground( wxSTC_B_NUMBER,rgb_digit );

	text->SetKeyWords( 0,keywordsList );

	text->Bind( wxEVT_STC_CHANGE,&FileView::OnTextEvent,this,wxID_ANY );

	wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
	sizer->Add( text,1,wxEXPAND,0 );
	SetSizer( sizer );
}

wxString FileView::GetTitle(){
	if ( path.length() == 0 ){
		return wxT( "<untitled>" );
	} else {
		return wxFileName( path ).GetFullName();
	}
}

wxString FileView::GetPath(){
	return path;
}

bool FileView::IsDirty(){
	return dirty;
}

void FileView::Open( wxString &path ){
	source.Clear();

	wxFileInputStream input( path );
	wxTextInputStream stream( input );
	while( input.IsOk()&&!input.Eof() ){
		source.Append( stream.ReadLine() );
		source.Append( "\n" );
	}

	text->SetText( source );
}

bool FileView::Save(){
	if( !dirty ) return true;

	if( path.length()==0 ){
		wxFileDialog dialog(this, wxT("Save Blitz Basic file..."), "", "", "Blitz Basic files (*.bb)|*.bb", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

		if (dialog.ShowModal() == wxID_CANCEL)
			return false;

		path=dialog.GetPath();
	}

	{
		wxFileOutputStream stream( path );
		if( !stream.IsOk() ){
			wxLogError("Cannot save current contents in file '%s'.", path);
			return false;
		}

		wxString out=text->GetText();
		stream.GetFile()->Write( out );

		// TODO: make preference option
		if( out[out.length()-1] != '\n' ){
			stream.GetFile()->Write( '\n' );
		}
	}

	source=text->GetText();

	dirty=false;
	EmitDirtyEvent();

	return true;
}

void FileView::OnTextEvent( wxStyledTextEvent& event ){
	dirty=true;
	EmitDirtyEvent();
}

void FileView::Execute( const Target &target,const Preferences *prefs ){
	cc=new BlitzCC( GetParent(),blitzpath );
	cc->Execute( path,target,prefs );
}

void FileView::Kill(){
	if( cc ){
		cc->Kill();
	}
}

void FileView::Build( wxString &out ){
	wxProcess *build=new wxProcess( this );
	build->Redirect();
	wxExecute( blitzpath + "/bin/blitzcc -o "+out+" "+path,wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE,build );
}

void FileView::EmitDirtyEvent(){
	wxCommandEvent event( FILE_VIEW_DIRTY_EVENT,GetId() );
	event.SetEventObject( this );
	event.SetInt( dirty?1:0 );
	ProcessWindowEvent( event );
}
