
#include "../../stdutil/stdutil.h"
#include "../../debugger/debugger.h"
using namespace std;

class ConsoleDebugger : public Debugger{
public:
	virtual void debugRun(){}
	virtual void debugStop(){}// bbruntime_panic(0); }
	virtual void debugStmt( int srcpos,const char *file ){}
	virtual void debugEnter( void *frame,void *env,const char *func ){}
	virtual void debugLeave(){}
	virtual void debugLog( const char *msg ){
    cout<<msg<<endl;
  }
	virtual void debugMsg( const char *e,bool serious ){
		cerr<<e<<endl;
	}
	virtual void debugSys( void *msg ){}
};

extern "C" Debugger * _cdecl debuggerGetDebugger( void *mod,void *env ){
	return d_new ConsoleDebugger();
}
