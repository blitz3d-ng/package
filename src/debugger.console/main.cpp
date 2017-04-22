
#include "../../stdutil/stdutil.h"
#include "../../debugger/debugger.h"
using namespace std;

class ConsoleDebugger : public Debugger{
public:
	void debugRun(){}
	void debugStop(){}// bbruntime_panic(0); }
	void debugStmt( int srcpos,const char *file ){}
	void debugEnter( void *frame,void *env,const char *func ){}
	void debugLeave(){}
	void debugLog( const char *msg ){
    cout<<msg<<endl;
  }
	void debugMsg( const char *e,bool serious ){
		cerr<<e<<endl;
	}
	void debugSys( void *msg ){}
};

extern "C" Debugger * __cdecl debuggerGetDebugger( void *mod,void *env ){
	return d_new ConsoleDebugger();
}
