
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "editor.h"
#include "htmlhelp.h"
#include "tabber.h"
#include "libs.h"

class MainFrame : public CFrameWnd,public HelpListener,EditorListener,TabberListener{
public:
	MainFrame();

	Editor *getEditor();
	void setTitle( const std::string &s );

	DECLARE_DYNAMIC( MainFrame )
	DECLARE_MESSAGE_MAP()

	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd( CDC *dc );
	afx_msg void OnSize( UINT type,int w,int h );
	afx_msg void quick_Help();

	afx_msg void fileNew();
	afx_msg void fileOpen();
	afx_msg void fileClose();
	afx_msg void fileCloseAll();
	afx_msg void fileSave();
	afx_msg void fileSaveAs();
	afx_msg void fileSaveAll();
	afx_msg void filePrint();
	afx_msg void fileExit();
	afx_msg void fileRecent( UINT id );

	afx_msg void editCut();
	afx_msg void editCopy();
	afx_msg void editPaste();
	afx_msg void editSelectAll();
	afx_msg void editFind();
	afx_msg void editFindNext();
	afx_msg void editReplace();

	afx_msg void programExecute();
	afx_msg void programReExecute();
	afx_msg void programCompile();
	afx_msg void programPublish();
	afx_msg void programRuntime( UINT id );
	afx_msg void programCommandLine();
	afx_msg void programDebug();

	afx_msg void helpHome();
	afx_msg void helpAutodoc();
	afx_msg void helpBack();
	afx_msg void helpForward();
	afx_msg void helpSupport();
	afx_msg void helpAbout();

	afx_msg void logSyn();
	afx_msg void logIR();
	afx_msg void logASM();
	afx_msg void logMsgs();
	afx_msg void noExecute();

	afx_msg void updateCmdUI( CCmdUI *ui );
	afx_msg void updateCmdUIRange( CCmdUI *ui );
	afx_msg void updateRuntimeCmdUI( CCmdUI *ui );
	afx_msg void ctrlTab();
	afx_msg void ctrlShiftTab();
	afx_msg void escape();
	afx_msg void OnActivate( UINT state,CWnd *other,BOOL min );

private:
	Tabber tabber;
	CToolBar toolBar;
	CStatusBar statusBar;

	std::map<CWnd*,Editor*> editors;
	std::map<CWnd*,HelpView*> helps;

	std::string last_quick_help;

	Runtime rt;

	HelpView *getHelp();
	HelpView *getHelp( int index );
	HelpView *findHelp();

	Editor *getEditor( int index );

	bool exit_flag;

	void insertRecent( const std::string &f );
	void newed( const std::string &t );
	bool load( const std::string &t,const std::string &f );
	bool open( const std::string &f );
	bool close( int n );
	bool save( int n );

	void compile( const std::string &cmd );
	void build( bool exec,bool publish );
	void switchRuntime( int i );

	//editorlistener
	void cursorMoved( Editor *editor );

	//tabberlistener
	void currentSet( Tabber *tabber,int index );

	//htmlhelplistener
	void helpOpen( HelpView *help,const std::string &file,bool dir );
	void helpTitleChange( HelpView *help,const std::string &title );
};

#endif
