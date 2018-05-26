
#include "../../stdutil/stdutil.h"
#include "runtime.html.h"

#include <bb/blitz2d.gl/blitz2d.gl.h>
#include <bb/blitz3d.gl/blitz3d.gl.h>

class HtmlRuntime : public BBRuntime, public BBContextDriver, public GLB3DGraphics{
public:
	~HtmlRuntime(){
		bbContextDriver=this;
		bbSceneDriver=this;
	}

	// context

	int numGraphicsDrivers(){
		return 1;
	}
	void graphicsDriverInfo( int driver,std::string *name,int *c ){
	}

	int numGraphicsModes( int driver ){
		return 1;
	}

	void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
	}

	void windowedModeInfo( int *c ){
	}

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags ){
		return 0;
	}

	void closeGraphics( BBGraphics *graphics ){
	}

	bool graphicsLost(){
		return false;
	}

	void flip( bool vwait ){
	}

	// runtime

	void asyncStop(){}
	void asyncRun(){}
	void asyncEnd(){}

	bool idle(){ return true; }

	void *window(){ return 0; }

	void moveMouse( int x,int y ){}
	void setPointerVisible( bool vis ){}
};

BBRuntime *bbCreateHTMLRuntime(){
	return d_new HtmlRuntime();
}

BBMODULE_CREATE( runtime_html ){
	return true;
}

BBMODULE_DESTROY( runtime_html ){
	return true;
}
