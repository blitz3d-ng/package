
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

void bbMain();

int main( int argc,char *argv[] ){
#ifdef DEBUG
  bb_env.debug=true;
#else
  bb_env.debug=false;
#endif

  string params;
  for( int i=1;i<argc;i++ ) params+=argv[i];
  bbStartup( params );

  bbAttachDebugger( 0 );

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
