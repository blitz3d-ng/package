#include <bb/stub/stub.h>
#include <bb/runtime/runtime.h>
#include <bb/blitz/blitz.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

#include <emscripten.h>

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
	}
	void debugSys( void *msg ){
	}
};

extern "C" void bbMain();

extern "C" int EMSCRIPTEN_KEEPALIVE main(){
	bbStartup( "","" );

	bool trace=false;
	StdioDebugger debugger( trace );
	bbAttachDebugger( &debugger );

#ifdef BB_DEBUG
  bool debug=true;
#else
  bool debug=false;
#endif
	return bbruntime_run( bbMain,debug )!=0;
}
