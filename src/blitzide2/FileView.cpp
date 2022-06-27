
#include "FileView.h"
#include <wx/stc/stc.h>
#include <wx/textfile.h>

enum {
  MARGIN_LINE_NUMBERS
};

static wxString keywordsList;
static bool keywordsLoaded=false;

extern wxString blitzpath;

void FileView::LoadKeywords(){
  if (!keywordsLoaded) {
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


FileView::FileView( wxString &path,wxWindow *parent,wxWindowID id ):path(path),wxPanel( parent,id ){
  LoadKeywords();

  // Inconsolata, Monaco, Consolas, 'Courier New', Courier
  wxFont font;
#ifdef __WXMSW__
  font = wxFontInfo(12).FaceName("Consolas");
#else
  font = wxFontInfo(12).FaceName("Monaco");
#endif

  wxStyledTextCtrl* text = new wxStyledTextCtrl(this, wxID_ANY);

  if ( path.length()>0 ){
    wxTextFile file;
    file.Open( path );

    source.Clear();

    while( !file.Eof() ) {
      source.Append( file.GetNextLine() );
      source.Append( "\n" );
    }

    file.Close();

    text->SetText( source );
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

  text->SetMarginWidth (MARGIN_LINE_NUMBERS, 25);

  text->SetCaretForeground( wxColour( 0xff,0xff,0xff ) );
  text->SetCaretLineBackground( wxColour( 0x1e,0x4a,0x76 ) );
  text->SetCaretLineVisible( true );

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

  wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
  sizer->Add( text,1,wxEXPAND,0 );
  SetSizer( sizer );
}

bool FileView::Save(){
  wxTextFile file( path );
  file.Open();

  file.AddLine( source );

  file.Write();
  file.Close();

  return true;
}

void FileView::Execute(){
	wxArrayString keywords;
	wxExecute( blitzpath + "/bin/blitzcc "+path );
}

void FileView::Build( wxString &out ){
	wxExecute( blitzpath + "/bin/blitzcc -o "+out+" "+path );
}
