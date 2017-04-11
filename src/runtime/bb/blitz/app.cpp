
#include "../../stdutil/stdutil.h"
#include "app.h"
#include "ex.h"
using namespace std;

static BBApp app;
BBHook bbAppOnChange;

void BBCALL bbEnd(){
	RTEX( 0 );
}

void BBCALL bbAppTitle( BBStr *ti,BBStr *cp ){
  app.title=*ti;
  app.close=*cp;

  BBApp tmp=app;
  bbAppOnChange.run( &tmp );

	delete ti;delete cp;
}

BBStr * BBCALL bbCommandLine(){
	return d_new BBStr( app.cmd_line );
}

void BBCALL bbStartup( string cmd_line ){
  app.cmd_line=cmd_line;
}

BBApp BBCALL bbApp(){
  return app;
}
