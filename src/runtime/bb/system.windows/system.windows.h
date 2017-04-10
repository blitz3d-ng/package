#ifndef BB_SYSTEM_WINDOWS_H
#define BB_SYSTEM_WINDOWS_H

#include <bb/system/system.h>
#include <bb/bank/bank.h>
#include <string>

class WindowsSystemDriver : public BBSystemDriver{
public:
	WindowsSystemDriver();
	~WindowsSystemDriver();

	bool delay( int ms );

	bool execute( const std::string &cmd );
	int getMilliSecs();

	int getScreenWidth( int i );
	int getScreenHeight( int i );

	int callDll( const std::string &dll,const std::string &func,const void *in,int in_sz,void *out,int out_sz );
};

int BBCALL bbCallDLL( BBStr *dll,BBStr *fun,bbBank *in,bbBank *out );

#endif
