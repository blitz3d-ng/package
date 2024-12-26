#include "CodePanel.h"

CodePanel::CodePanel( wxWindow *parent )
	: wxPanel( parent )
{
	nb=new wxNotebook( this,wxID_ANY );

	functionList = new wxListBox( nb,wxID_ANY );
	nb->AddPage( functionList,wxT("Functions") );

	typeList = new wxListBox( nb,wxID_ANY );
	nb->AddPage( typeList,wxT("Types") );

	labelList = new wxListBox( nb,wxID_ANY );
	nb->AddPage( labelList,wxT("Labels") );

	wxBoxSizer *sizer=new wxBoxSizer( wxVERTICAL );
	sizer->Add( nb,1,wxEXPAND,0 );

	SetSizer( sizer );
}

void CodePanel::updateLists( wxString source ){
	functions.clear();
	types.clear();
	labels.clear();

	functionList->Clear();
	typeList->Clear();
	labelList->Clear();

	int line=0;
	wxString str="";
	wxString::const_iterator i;
	for( i=source.begin();i!=source.end();++i ){
		wxUniChar ch = *i;

		if( ch==10||ch==13 ){
			str=str.Trim();

			if( str.Lower().StartsWith("function") ){
				str = str.Mid( 8 );
				while( str.GetChar(0)==' '||str.GetChar(0)=='\t' ) str=str.Mid(1);

				int idx=functionList->GetCount();
				functionList->InsertItems( 1,&str,idx );
				functions[idx]=line;
			}else if( str.Lower().StartsWith("type") ){
				str = str.Mid( 4 );
				while( str.GetChar(0)==' '||str.GetChar(0)=='\t' ) str=str.Mid(1);

				int idx=typeList->GetCount();
				typeList->InsertItems( 1,&str,idx );
				types[idx]=line;
			}else if( str.Lower().StartsWith(".") ){
				str = str.Mid( 1 );
				while( str.GetChar(0)==' '||str.GetChar(0)=='\t' ) str=str.Mid(1);

				int idx=labelList->GetCount();
				labelList->InsertItems( 1,&str,idx );
				labels[idx]=line;
			}

			str="";
			line+=1;
			continue;
		}

		str += ch;
	}
}
