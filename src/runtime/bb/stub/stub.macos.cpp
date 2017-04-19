
#include <bb/stub/stub.h>
#include <bb/runtime/runtime.h>
#include <bb/blitz/blitz.h>

#include <iostream>
#include <cstdlib>
using namespace std;

void sue( const char *t ){
  cerr<<t<<endl;
  exit(1);
}

class StdioDebugger : public Debugger{
public:
	virtual void debugRun(){
	}
	virtual void debugStop(){
	}
	virtual void debugStmt( int srcpos,const char *file ){
		cout<<file<<":"<<srcpos<<endl;
	}
	virtual void debugEnter( void *frame,void *env,const char *func ){
	}
	virtual void debugLeave(){
	}
	virtual void debugLog( const char *msg ){
		cout<<msg<<endl;
	}
	virtual void debugMsg( const char *msg,bool serious ){
	}
	virtual void debugSys( void *msg ){
	}
};

extern "C" void bbMain();

int main( int argc,char *argv[] ){
#ifdef DEBUG
  bb_env.debug=true;
#else
  bb_env.debug=false;
#endif

  string params;
  for( int i=1;i<argc;i++ ) params+=argv[i];
  bbStartup( params );

	StdioDebugger debugger;
  bbAttachDebugger( &debugger );

  if( !(bbRuntime=bbCreateRuntime()) ){
    cerr<<"Failed to create runtime"<<endl;
    return 1;
  }

  int retcode=0;
  try{
    if( !bbruntime_create() ) return 1;
    bbMain();
  }catch( bbEx ex ){
    cerr<<ex.err<<endl;
    retcode=1;
  }
  bbruntime_destroy();
  return retcode;
}
