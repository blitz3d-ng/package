#include <bb/stub/stub.h>
#include <bb/runtime/runtime.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>

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
		if( serious ){
			std::cout<<file<<":"<<"["<<row+1<<":"<<col<<"] "<<msg<<std::endl;
		}
	}
	void debugSys( void *msg ){
		std::cout<<file<<":"<<"["<<row+1<<":"<<col<<"] "<<msg<<std::endl;
	}
};

extern "C"
int BBCALL bbStart( int argc,char *argv[], BBMAIN bbMain ) {
	std::string cmd_line="";
	bbStartup( argv[0],cmd_line.c_str() );

	bool debug=false;
	StdioDebugger debugger( debug );
	bbAttachDebugger( &debugger );

	return bbruntime_run( bbMain,debug )?0:1;
}
