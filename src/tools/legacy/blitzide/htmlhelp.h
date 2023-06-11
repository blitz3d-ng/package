
#ifndef HTMLHELP_H
#define HTMLHELP_H

class HelpView;

class HelpListener{
public:
	virtual void helpOpen( HelpView *help,const std::string &file,bool dir )=0;
	virtual void helpTitleChange( HelpView *help,const std::string &title )=0;
};

class HelpView : public CHtmlView{
public:
	HelpView( HelpListener *l ):listener(l){}

	std::string getTitle();

DECLARE_DYNAMIC( HelpView )
DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEraseBkgnd( CDC *dc );

private:
	virtual void OnTitleChange( LPCTSTR t );
	virtual void OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel );

	std::string title;
	HelpListener *listener;
};

#endif
