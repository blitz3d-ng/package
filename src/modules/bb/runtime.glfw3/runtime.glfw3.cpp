
#include "../../stdutil/stdutil.h"
#include "runtime.glfw3.h"
#include <bb/pixmap/pixmap.h>
#include <bb/event/event.h>
#include <bb/system/system.h>
#include <bb/input/input.h>

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <map>
using namespace std;

map<GLFWwindow*,GLFW3Runtime*> runtimes;

BBRuntime *bbCreateGLFW3Runtime(){
	if( !glfwInit() ) return 0;

	glfwWindowHint( GLFW_FOCUSED,1 );
	glfwWindowHint( GLFW_RESIZABLE,0 );

	GLFWwindow* window=glfwCreateWindow( 1,1,"",NULL,NULL );
	if( !window ){
		glfwTerminate();
		return 0;
	}

	return d_new GLFW3Runtime( window );
}

void bbCloseRuntime( BBRuntime *rt ){
	delete rt;
}

void _onCharMods(GLFWwindow *wnd, unsigned int codepoint, int mods){
}

GLFW3Runtime::GLFW3Runtime( GLFWwindow *wnd ):wnd(wnd),graphics(0){
	bbContextDriver=this;
	bbSceneDriver=this;

	runtimes.insert( make_pair( wnd,this ) );

	glfwSetCursorPosCallback( wnd,_onMouseMove );
	glfwSetMouseButtonCallback( wnd,_onMouseButton );
	glfwSetKeyCallback( wnd,_onKeyChange );
	glfwSetCharModsCallback( wnd,_onCharMods );
	glfwSetFramebufferSizeCallback( wnd,_onResize );
	glfwSetWindowCloseCallback( wnd,_onClose );
}

GLFW3Runtime::~GLFW3Runtime(){
	glfwTerminate();
}

void GLFW3Runtime::_onClose( GLFWwindow *w ){
	RTEX( 0 );
}

int GLFW3Runtime::numGraphicsDrivers(){
	int count;
	glfwGetMonitors( &count );
	return count;
}

void GLFW3Runtime::graphicsDriverInfo( int driver,std::string *name,int *c ){
}

int GLFW3Runtime::numGraphicsModes( int driver ){
	int count;
	glfwGetVideoModes( glfwGetPrimaryMonitor(),&count );
	return count;
}

void GLFW3Runtime::graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
	int mcount,vcount;
	GLFWmonitor** monitors=glfwGetMonitors( &mcount );
	const GLFWvidmode* modes=glfwGetVideoModes( monitors[driver],&vcount );

	*w=modes[mode].width;
	*h=modes[mode].height;
	*d=modes[mode].redBits+modes[mode].greenBits+modes[mode].blueBits;
	*c=GFXMODECAPS_3D;
}

void GLFW3Runtime::windowedModeInfo( int *c ){
	*c=GFXMODECAPS_3D;
}

class GLFW3DefaultCanvas : public GLB2DDefaultCanvas{
protected:
	GLFWwindow *wnd;
public:
	GLFW3DefaultCanvas( GLFWwindow *wnd,int mode,int flags ):GLB2DDefaultCanvas(mode,flags),wnd(wnd){
	}

	// int getWidth()const{
	//   // int width,height;
	//   // glfwGetWindowSize( wnd,&width,&height );
	//   // return width;
	// }
	//
	// int getHeight()const{
	//   // int width,height;
	//   // glfwGetWindowSize( wnd,&width,&height );
	//   // return height;
	// }

	void getViewport( int *x,int *y,int *w,int *h )const{
		*x=0;*y=0;*w=getWidth();*h=getHeight();
	}
};

class GLFW3Graphics : public B2DGraphics{
protected:
	GLFWwindow *wnd;

	GLFWgammaramp gamma_ramp;
	unsigned short gamma_red[256], gamma_green[256], gamma_blue[256];

	BBImageFont *def_font;
public:
	GLFW3Graphics( GLFWwindow *wnd ):wnd(wnd){
		front_canvas=d_new GLFW3DefaultCanvas( wnd,GL_FRONT,0 );
		back_canvas=d_new GLFW3DefaultCanvas( wnd,GL_BACK,0 );

		def_font=(BBImageFont*)loadFont( "courier",12*bbDPIScaleY(),0 );

		gamma_ramp.size=256;
		gamma_ramp.red=gamma_red;
		gamma_ramp.green=gamma_green;
		gamma_ramp.blue=gamma_blue;
		for( int k=0;k<256;++k ) gamma_red[k]=gamma_green[k]=gamma_blue[k]=k;

		int w,h;
		glfwGetFramebufferSize( wnd,&w,&h );
		resize( w,h );
	}

	~GLFW3Graphics(){
		if( front_canvas ) delete front_canvas;
		if( back_canvas ) delete back_canvas;
		front_canvas=back_canvas=0;
	}

	void resize( int w,int h ){
		((GLB2DCanvas*)front_canvas)->resize( w,h );
		((GLB2DCanvas*)back_canvas)->resize( w,h );
	}

	// graphics
	void backup(){}
	bool restore(){ return true; }

	//MANIPULATORS
	void vwait(){}

	//SPECIAL!
	void copy( BBCanvas *dest,int dx,int dy,int dw,int dh,BBCanvas *src,int sx,int sy,int sw,int sh ){}

  //NEW! Gamma control!
	void setGamma( int r,int g,int b,float dr,float dg,float db ){
		gamma_red[r&255]=dr*257.0f;
		gamma_red[g&255]=dg*257.0f;
		gamma_red[b&255]=db*257.0f;
	}
	void getGamma( int r,int g,int b,float *dr,float *dg,float *db ){
		*dr=gamma_red[r&255];*dg=gamma_green[g&255];*db=gamma_blue[b&255];
	}

  void updateGamma( bool calibrate ){
		glfwSetGammaRamp( glfwGetPrimaryMonitor(), &gamma_ramp);
	}

	//ACCESSORS
	int getWidth()const{ return front_canvas->getWidth(); }
	int getHeight()const{ return front_canvas->getHeight(); }
	int getDepth()const{ return 0; }
	int getScanLine()const{ return 0; }
	int getAvailVidmem()const{ return 0; }
	int getTotalVidmem()const{ return 0; }

	BBFont *getDefaultFont()const{ return def_font; }

	//OBJECTS
	BBCanvas *createCanvas( int width,int height,int flags ){
		BBCanvas *canvas=d_new GLB2DTextureCanvas( width,height,flags );
		canvas_set.insert( canvas );
		return canvas;
	}

	BBCanvas *loadCanvas( const std::string &file,int flags ){
		BBPixmap *pixmap=bbLoadPixmap( file );
		if( !pixmap ) return 0;

		BBCanvas *canvas=d_new GLB2DTextureCanvas( pixmap,flags );
		canvas_set.insert( canvas );
		delete pixmap;
		return canvas;
	}

	// b2dgraphics
	BBMovie *openMovie( const std::string &file,int flags ){ return 0; }
	BBMovie *verifyMovie( BBMovie *movie ){ return 0; }
	void closeMovie( BBMovie *movie ){}

	BBFont *loadFont( const std::string &font,int height,int flags ){
		return BBImageFont::load( font,height,flags );
	}
	BBFont *verifyFont( BBFont *font ){
		return font;
	}
	void freeFont( BBFont *font ){
		delete font;
	}

	void moveMouse( int x,int y ){
		glfwSetCursorPos( wnd,x,y );
	}

	void setPointerVisible( bool vis ){
		glfwSetInputMode( wnd,GLFW_CURSOR,vis?GLFW_CURSOR_NORMAL:GLFW_CURSOR_HIDDEN );
	}
};

BBGraphics *GLFW3Runtime::openGraphics( int w,int h,int d,int driver,int flags ){
	if( graphics ) return 0;

	if( (graphics=d_new GLFW3Graphics( wnd )) ){
		const GLFWvidmode* mode=glfwGetVideoMode( glfwGetPrimaryMonitor() );
		glfwSetWindowPos( wnd,(mode->width-w)/2.0f,(mode->height-h)/2.0f );
		glfwSetWindowSize( wnd,w,h );
		glfwShowWindow( wnd );
		glfwMakeContextCurrent( wnd );

#if defined(__linux__) || defined(_WIN32) // TODO: move this elsewhere...
		int err=glewInit();
		if( err!=GLEW_OK ) RTEX( (const char*)glewGetErrorString(err) );
#endif

#ifdef WIN32
		glfwFocusWindow( wnd );
#endif
		return graphics;
	}
	return 0;
}

void GLFW3Runtime::closeGraphics( BBGraphics *g ){
	if( graphics!=g || !g ) return;
	delete graphics;graphics=0;
	glfwHideWindow( wnd );
}

bool GLFW3Runtime::graphicsLost(){
	return true;
}

void GLFW3Runtime::flip( bool vwait ){
	glfwSwapInterval( vwait ? 1 : 0 );
	glfwSwapBuffers( wnd );
}

void GLFW3Runtime::asyncStop(){
}

void GLFW3Runtime::asyncRun(){
}

void GLFW3Runtime::asyncEnd(){
}

bool GLFW3Runtime::idle(){
	glfwPollEvents();
	return true;
}

void *GLFW3Runtime::window(){
#ifdef WIN32
	return glfwGetWin32Window( wnd );
#else
	return 0;
#endif
}

void GLFW3Runtime::moveMouse( int x,int y ){
	graphics->moveMouse( x,y );
}

void GLFW3Runtime::setPointerVisible( bool vis ){
	graphics->setPointerVisible( vis );
}

void GLFW3Runtime::_onMouseMove( GLFWwindow *w,double x,double y ){
	int sw,sh;
	glfwGetWindowSize( w,&sw,&sh );
	BBEvent ev( BBEVENT_MOUSEMOVE,0,(x/sw)*bbGraphicsWidth(),(y/sh)*bbGraphicsHeight() );
	bbOnEvent.run( &ev );
}

void GLFW3Runtime::_onMouseButton( GLFWwindow *w,int button,int action,int mods ){
	BBEvent ev( action==GLFW_PRESS?BBEVENT_MOUSEDOWN:BBEVENT_MOUSEUP,button+1 );
	bbOnEvent.run( &ev );
}

extern unsigned short GLFW_SCANCODE_MAP[316];

void GLFW3Runtime::_onKeyChange( GLFWwindow *w,int key,int scancode,int action,int mods ){
	// cout<<"key: "<<key<<endl;
	key=GLFW_SCANCODE_MAP[key-32];
	if( !key ) return;

	BBEvent ev;
	switch( action ){
	case GLFW_PRESS:
		ev=BBEvent( BBEVENT_KEYDOWN,key );
		break;
	case GLFW_RELEASE:
		ev=BBEvent( BBEVENT_KEYUP,key );
		break;
	}
	if( ev.id ) bbOnEvent.run( &ev );
}

void GLFW3Runtime::_onResize( GLFWwindow *w,int width,int height ){
	GLFW3Runtime *rt=runtimes.at( w );
	rt->resize( width,height );
}

void GLFW3Runtime::resize( int width,int height ){
	if( graphics ) graphics->resize( width,height );
}

#include <bb/input/input.h>

class GLFWInputDriver : public BBInputDriver{
public:
	~GLFWInputDriver(){}

	BBDevice *getJoystick( int port )const{ return 0; }
	int getJoystickType( int port )const{ return 0; }
	int numJoysticks()const{ return 0; }

	int toAscii( int key )const{
		cout<<key<<endl;
		if( key==28 ) return '\n';
		return 65;
	}
};

class GLFWJoystick : public BBDevice{
private:
	int idx;
public:
	GLFWJoystick( int i ):idx(i){
		memset( axis_states,0,sizeof(axis_states) );
		memset( down_state,0,sizeof(down_state) );

		snprintf( id,sizeof(id),"%s",glfwGetJoystickName( idx ) );
		snprintf( name,sizeof(name),"%s",glfwGetJoystickName( idx ) );
	}

	void update(){
		int ax_count;
		const float *ax_state=glfwGetJoystickAxes( idx,&ax_count );
		if( ax_count>32 ) ax_count=32;
		memcpy( axis_states,ax_state,ax_count*sizeof(float) );

		int btn_count;
		const unsigned char *btn_state=glfwGetJoystickButtons( idx,&btn_count );
		for( int i=0;i<32;i++ ){
			setDownState( i,btn_state[i] );
		}
	}
};

#ifndef WIN32
#include <bb/timer/timer.h>

class GLFWTimer : public BBTimer{
protected:
	int hertz;
public:
	GLFWTimer( int h ):hertz(h){}
	~GLFWTimer(){}

	int wait(){
		return 0; // FIXME: implement this properly.
	}
};

BBTimer * BBCALL _bbCreateTimer( int hertz ){
	return new GLFWTimer( hertz );
}

#endif

BBMODULE_CREATE( runtime_glfw3 ){
	gx_input=d_new GLFWInputDriver();

	for( int i=0;i<GLFW_JOYSTICK_LAST;i++ ){
		if( glfwJoystickPresent( GLFW_JOYSTICK_1+i ) ){
			GLFWJoystick *js=d_new GLFWJoystick( GLFW_JOYSTICK_1+i );
			bbJoysticks.push_back( js );
		}
	}

	return true;
}

BBMODULE_DESTROY( runtime_glfw3 ){
	return true;
}
