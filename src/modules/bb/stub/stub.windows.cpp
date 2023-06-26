
#include <bb/stub/stub.h>
#include <bb/runtime/runtime.h>

#include <string>

extern "C" int BBCALL bbStart( int argc,char *argv[], BBMAIN bbMain ) {
	std::string cmd_line;
	for( int i=1;i<argc;i++ ){
		cmd_line+=argv[i];
	}
	bbStartup( argv[0],cmd_line.c_str() );
	return bbruntime_run( bbMain,true )?0:1;
};

void bbruntime_panic( const char *err ){
	RTEX( err );
}
