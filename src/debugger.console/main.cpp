
#include "../../stdutil/stdutil.h"
#include <bb/blitz/debug.h>
using namespace std;

class ConsoleDebugger : public Debugger{
protected:
	string file;
	int row,col;
public:
	void debugRun(){}
	void debugStop(){}// bbruntime_panic(0); }
	void debugStmt( int srcpos,const char *f ){
		file=f;
		row=(srcpos>>16)&0xffff;
		col=srcpos&0xffff;
	}
	void debugEnter( void *frame,void *env,const char *func ){}
	void debugLeave(){}
	void debugLog( const char *msg ){
    cout<<msg<<endl;
  }
	void debugMsg( const char *e,bool serious ){
		cerr<<file<<"["<<row<<","<<col<<"]: "<<e<<endl;
	}
	void debugSys( void *msg ){}
};

extern "C" Debugger * __cdecl debuggerGetDebugger( void *mod,void *env ){
	return d_new ConsoleDebugger();
}
