
#ifndef PREFS_H
#define PREFS_H

class Prefs{
public:
	bool prg_debug;
	std::string prg_lastbuild;

	RECT win_rect;
	bool win_maximized;
	bool win_notoolbar;

	std::string font_editor,font_tabs,font_debug;
	int font_editor_height,font_tabs_height,font_debug_height;

	int rgb_bkgrnd;		//0
	int rgb_string;		//1
	int rgb_ident;		//2
	int rgb_keyword;	//3
	int rgb_comment;	//4
	int rgb_digit;		//5
	int rgb_default;	//6

	int edit_tabs;
	bool edit_blkcursor;
	int edit_backup;

	std::string img_toolbar;

	std::string homeDir;
	CFont conFont,editFont,tabsFont,debugFont;

	std::vector<std::string> recentFiles;

	std::string cmd_line;

	void open();
	void close();

private:

	void setDefault();
	void createFonts();
};

extern Prefs prefs;

#endif