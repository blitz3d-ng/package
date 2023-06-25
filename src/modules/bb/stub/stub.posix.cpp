
#include <bb/stub/stub.h>
#include <bb/runtime/runtime.h>
#include <bb/blitz/blitz.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

#ifndef BB_NDK
#ifdef BB_DEBUG
#define BB_BACKTRACE
#endif
#endif

#include <signal.h>
#include <unistd.h>
#ifndef __BIONIC__
#include <execinfo.h>
#endif
#ifdef BB_MACOS
#include <libgen.h>
#include <mach-o/dyld.h>
#endif

class StdioDebugger : public Debugger{
private:
	bool trace;
	std::string file;
	int row,col;
public:
	StdioDebugger( bool t ):trace(t){
	}

	void debugRun(){
	}
	void debugStop(){
	}
	void debugStmt( int srcpos,const char *f ){
		file=std::string(f);
		row=(srcpos>>16)&0xffff;
		col=srcpos&0xffff;
		if( trace ) std::cout<<file<<":"<<"["<<row+1<<":"<<col<<"]"<<std::endl;
	}
	void debugEnter( void *frame,void *env,const char *func ){
	}
	void debugLeave(){
	}
	void debugLog( const char *msg ){
    std::cout<<file<<":"<<"["<<row+1<<":"<<col<<"] "<<msg<<std::endl;
	}
	void debugMsg( const char *msg,bool serious ){
		std::cout<<file<<":"<<"["<<row+1<<":"<<col<<"] "<<msg<<std::endl;
	}
	void debugSys( void *msg ){
		std::cout<<file<<":"<<"["<<row+1<<":"<<col<<"] "<<msg<<std::endl;
	}
};

#ifdef BB_BACKTRACE
#define BACKTRACE_DEPTH 10
static
void dump_backtrace(int sig) {
	void *array[BACKTRACE_DEPTH];

	size_t size=backtrace( array,BACKTRACE_DEPTH );

	fprintf(stderr, "Error (%d):\n", sig);
	backtrace_symbols_fd( array,size,STDERR_FILENO );

	// exit( 1 );
}
#endif

extern "C"
int BBCALL bbStart( int argc,char *argv[], BBMAIN bbMain ) {
#ifdef BB_BACKTRACE
	// signal(SIGSEGV, dump_backtrace);
#endif

#ifdef BB_MACOS
	// TODO: this is a little bit of a hack...but it should work for now.
	char path[PATH_MAX];
	uint32_t path_len=sizeof( path );
	_NSGetExecutablePath( path,&path_len );
	std::string dir=dirname( path );
	if( dir.substr( dir.length()-4 )==".app" ){
		chdir( dir.c_str() );
	}
#endif

	bool trace=false;
	std::string cmd_line;
	for( int i=1;i<argc;i++ ){
		if( strncmp( "--trace",argv[i],strlen("--trace")+1 )==0 ){
			trace=true;
		}
		cmd_line+=argv[i];
	}
	bbStartup( argv[0],cmd_line.c_str() );

	StdioDebugger debugger( trace );
	bbAttachDebugger( &debugger );

#ifdef BB_DEBUG
  bool debug=true;
#else
  bool debug=false;
#endif
	return bbruntime_run( bbMain,debug )?0:1;
}
