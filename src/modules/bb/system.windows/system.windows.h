#ifndef BB_SYSTEM_WINDOWS_H
#define BB_SYSTEM_WINDOWS_H

#include <bb/system/system.h>
#include <bb/bank/bank.h>
#include <windows.h>
#include <string>

class WindowsSystemDriver : public BBSystemDriver{
private:
	OSVERSIONINFO osinfo;

	void refreshSystemProperties();
public:
	bool isXPorLess();

public:
	WindowsSystemDriver();
	~WindowsSystemDriver();

	bool delay( int ms );

	bool execute( const std::string &cmd );
	int getMilliSecs();

	int getScreenWidth( int i );
	int getScreenHeight( int i );

	void dpiInfo( float &scale_x,float &scale_y );

	bool lookupFontData( const std::string &fontName,BBFontData &font );

	int callDll( const std::string &dll,const std::string &func,const void *in,int in_sz,void *out,int out_sz );
};

#include "commands.h"

#define bbWindowsSystemDriver ((WindowsSystemDriver*)bbSystemDriver)

#endif
