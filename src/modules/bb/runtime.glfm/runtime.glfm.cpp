
#include "../../stdutil/stdutil.h"
#include "runtime.glfm.h"
#include <bb/pixmap/pixmap.h>
#include <bb/event/event.h>
#include <bb/system/system.h>
#include <bb/input/input.h>

#include <map>
using namespace std;

map<GLFMDisplay*,GLFMRuntime*> runtimes;

// TODO: remove this...
BBRuntime *bbCreateOpenGLRuntime(){
	return 0;
}

void bbCloseRuntime( BBRuntime *rt ){
	delete rt;
}

// void _onCharMods(GLFWwindow *wnd, unsigned int codepoint, int mods){
// }
//
GLFMRuntime::GLFMRuntime( GLFMDisplay *display ):display(display){
	bbContextDriver=this;
	// bbSceneDriver=this;

	runtimes.insert( make_pair( display,this ) );

	// glfwSetCursorPosCallback( wnd,_onMouseMove );
	// glfwSetMouseButtonCallback( wnd,_onMouseButton );
	// glfwSetKeyCallback( wnd,_onKeyChange );
	// glfwSetCharModsCallback( wnd,_onCharMods );
	// glfwSetFramebufferSizeCallback( wnd,_onResize );
	// glfwSetWindowCloseCallback( wnd,_onClose );
}

GLFMRuntime::~GLFMRuntime(){
}

// void GLFMRuntime::_onClose( GLFWwindow *w ){
// 	RTEX( 0 );
// }

int GLFMRuntime::numGraphicsDrivers(){
	return 1;
}

void GLFMRuntime::graphicsDriverInfo( int driver,std::string *name,int *c ){
}

int GLFMRuntime::numGraphicsModes( int driver ){
	return 1;
}

void GLFMRuntime::graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
	*w=0;
	*h=0;
	*d=0;
	*c=GFXMODECAPS_3D;
}

void GLFMRuntime::windowedModeInfo( int *c ){
	*c=GFXMODECAPS_3D;
}

class GLFMDefaultCanvas : public GLES2B2DDefaultCanvas{
protected:
	GLFMDisplay *display;
public:
	GLFMDefaultCanvas( GLFMDisplay *display,int mode,int flags ):GLES2B2DDefaultCanvas(mode,flags),display(display){
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

class GLFMGraphics : public B2DGraphics{
protected:
	GLFMDisplay *display;

	// GLFWgammaramp gamma_ramp;
	unsigned short gamma_red[256], gamma_green[256], gamma_blue[256];

	BBImageFont *def_font;
public:
	GLFMGraphics( GLFMDisplay *display ):display(display){
		front_canvas=d_new GLFMDefaultCanvas( display,GL_FRONT,0 );
		back_canvas=d_new GLFMDefaultCanvas( display,GL_BACK,0 );

		def_font=(BBImageFont*)loadFont( "courier",12*bbDPIScaleY(),0 );

		// gamma_ramp.size=256;
		// gamma_ramp.red=gamma_red;
		// gamma_ramp.green=gamma_green;
		// gamma_ramp.blue=gamma_blue;
		for( int k=0;k<256;++k ) gamma_red[k]=gamma_green[k]=gamma_blue[k]=k;

		// int w,h;
		// glfwGetFramebufferSize( wnd,&w,&h );
		// resize( w,h );
	}

	~GLFMGraphics(){
		if( front_canvas ) delete front_canvas;
		if( back_canvas ) delete back_canvas;
		front_canvas=back_canvas=0;
	}

	void resize( int w,int h ){
		((GLES2B2DCanvas*)front_canvas)->resize( w,h );
		((GLES2B2DCanvas*)back_canvas)->resize( w,h );
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
		// glfwSetGammaRamp( glfwGetPrimaryMonitor(), &gamma_ramp);
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
		BBCanvas *canvas=d_new GLES2B2DTextureCanvas( width,height,flags );
		canvas_set.insert( canvas );
		return canvas;
	}

	BBCanvas *loadCanvas( const std::string &file,int flags ){
		BBPixmap *pixmap=bbLoadPixmap( file );
		if( !pixmap ) return 0;

		BBCanvas *canvas=d_new GLES2B2DTextureCanvas( pixmap,flags );
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
		// glfwSetCursorPos( wnd,x,y );
	}

	void setPointerVisible( bool vis ){
		// glfwSetInputMode( wnd,GLFW_CURSOR,vis?GLFW_CURSOR_NORMAL:GLFW_CURSOR_HIDDEN );
	}
};

BBGraphics *GLFMRuntime::openGraphics( int w,int h,int d,int driver,int flags ){
	if( graphics ) return 0;

	if( (graphics=d_new GLFMGraphics( display )) ){
		// const GLFWvidmode* mode=glfwGetVideoMode( glfwGetPrimaryMonitor() );
		// glfwSetWindowPos( wnd,(mode->width-w)/2.0f,(mode->height-h)/2.0f );
		// glfwSetWindowSize( wnd,w,h );
		// glfwShowWindow( wnd );
		// glfwMakeContextCurrent( wnd );

		return graphics;
	}
}

void GLFMRuntime::closeGraphics( BBGraphics *g ){
}

bool GLFMRuntime::graphicsLost(){
	return true;
}

void GLFMRuntime::flip( bool vwait ){
}

void GLFMRuntime::asyncStop(){
}

void GLFMRuntime::asyncRun(){
}

void GLFMRuntime::asyncEnd(){
}

bool GLFMRuntime::idle(){
	return true;
}

void *GLFMRuntime::window(){
	return 0;
}

void GLFMRuntime::moveMouse( int x,int y ){
}

void GLFMRuntime::setPointerVisible( bool vis ){
}

// void GLFMRuntime::_onMouseMove( GLFWwindow *w,double x,double y ){
// 	int sw,sh;
// 	glfwGetWindowSize( w,&sw,&sh );
// 	BBEvent ev( BBEVENT_MOUSEMOVE,0,(x/sw)*bbGraphicsWidth(),(y/sh)*bbGraphicsHeight() );
// 	bbOnEvent.run( &ev );
// }
//
// void GLFMRuntime::_onMouseButton( GLFWwindow *w,int button,int action,int mods ){
// 	BBEvent ev( action==GLFW_PRESS?BBEVENT_MOUSEDOWN:BBEVENT_MOUSEUP,button+1 );
// 	bbOnEvent.run( &ev );
// }
//
// extern unsigned short GLFW_SCANCODE_MAP[316];
//
// void GLFMRuntime::_onKeyChange( GLFWwindow *w,int key,int scancode,int action,int mods ){
// 	// cout<<"key: "<<key<<endl;
// 	key=GLFW_SCANCODE_MAP[key-32];
// 	if( !key ) return;
//
// 	BBEvent ev;
// 	switch( action ){
// 	case GLFW_PRESS:
// 		ev=BBEvent( BBEVENT_KEYDOWN,key );
// 		break;
// 	case GLFW_RELEASE:
// 		ev=BBEvent( BBEVENT_KEYUP,key );
// 		break;
// 	}
// 	if( ev.id ) bbOnEvent.run( &ev );
// }
//
// void GLFMRuntime::_onResize( GLFWwindow *w,int width,int height ){
// 	GLFMRuntime *rt=runtimes.at( w );
// 	rt->resize( width,height );
// }
//
// void GLFMRuntime::resize( int width,int height ){
// 	if( graphics ) graphics->resize( width,height );
// }
//
// #include <bb/input/input.h>
//
// class GLFWInputDriver : public BBInputDriver{
// public:
// 	~GLFWInputDriver(){}
//
// 	BBDevice *getJoystick( int port )const{ return 0; }
// 	int getJoystickType( int port )const{ return 0; }
// 	int numJoysticks()const{ return 0; }
//
// 	int toAscii( int key )const{
// 		cout<<key<<endl;
// 		if( key==28 ) return '\n';
// 		return 65;
// 	}
// };
//
// class GLFWJoystick : public BBDevice{
// private:
// 	int idx;
// public:
// 	GLFWJoystick( int i ):idx(i){
// 		memset( axis_states,0,sizeof(axis_states) );
// 		memset( down_state,0,sizeof(down_state) );
//
// 		snprintf( id,sizeof(id),"%s",glfwGetJoystickName( idx ) );
// 		snprintf( name,sizeof(name),"%s",glfwGetJoystickName( idx ) );
// 	}
//
// 	void update(){
// 		int ax_count;
// 		const float *ax_state=glfwGetJoystickAxes( idx,&ax_count );
// 		if( ax_count>32 ) ax_count=32;
// 		memcpy( axis_states,ax_state,ax_count*sizeof(float) );
//
// 		int btn_count;
// 		const unsigned char *btn_state=glfwGetJoystickButtons( idx,&btn_count );
// 		for( int i=0;i<32;i++ ){
// 			setDownState( i,btn_state[i] );
// 		}
// 	}
// };
//
// #ifndef WIN32
// #include <bb/timer/timer.h>
//
// class GLFWTimer : public BBTimer{
// protected:
// 	int hertz;
// public:
// 	GLFWTimer( int h ):hertz(h){}
// 	~GLFWTimer(){}
//
// 	int wait(){
// 		return 0; // FIXME: implement this properly.
// 	}
// };
//
// BBTimer * BBCALL _bbCreateTimer( int hertz ){
// 	return new GLFWTimer( hertz );
// }
//
// #endif

BBMODULE_CREATE( runtime_glfm ){
	// gx_input=d_new GLFWInputDriver();
	//
	// for( int i=0;i<GLFW_JOYSTICK_LAST;i++ ){
	// 	if( glfwJoystickPresent( GLFW_JOYSTICK_1+i ) ){
	// 		GLFWJoystick *js=d_new GLFWJoystick( GLFW_JOYSTICK_1+i );
	// 		bbJoysticks.push_back( js );
	// 	}
	// }

	return true;
}

BBMODULE_DESTROY( runtime_glfm ){
	return true;
}
