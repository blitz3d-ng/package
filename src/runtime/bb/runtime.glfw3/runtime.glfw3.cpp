
#include "../../stdutil/stdutil.h"
#include "runtime.glfw3.h"
#include <bb/blitz2d/blitz2d.h>

#include <map>
using namespace std;

map<GLFWwindow*,GLFW3Runtime*> runtimes;

BBRuntime *bbCreateRuntime(){
  if( !glfwInit() ) return 0;

  glfwWindowHint( GLFW_FOCUSED,1 );

  GLFWwindow* window=glfwCreateWindow( 1,1,"",NULL,NULL );
  if( !window ){
    glfwTerminate();
    return 0;
  }

  return d_new GLFW3Runtime( window );
}

GLFW3Runtime::GLFW3Runtime( GLFWwindow *wnd ):wnd(wnd),graphics(0){
  bbContextDriver=this;
  bbSceneDriver=this;

  runtimes.insert( pair<GLFWwindow*,GLFW3Runtime*>( wnd,this ) );

  glfwSetFramebufferSizeCallback( wnd,_onResize );
  glfwSetWindowCloseCallback( wnd,_onClose );
}

GLFW3Runtime::~GLFW3Runtime(){
  glfwTerminate();
}

void GLFW3Runtime::_onClose( GLFWwindow *w ){
  // RTEX( 0 );
  exit(1); // FIXME: need to get exception handling working...
}

int GLFW3Runtime::numGraphicsDrivers(){
  return 0;
}

void GLFW3Runtime::graphicsDriverInfo( int driver,std::string *name,int *c ){
}

int GLFW3Runtime::numGraphicsModes( int driver ){
  return 0;
}

void GLFW3Runtime::graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
}

void GLFW3Runtime::windowedModeInfo( int *c ){
}

class GLFW3Canvas : public BBCanvas{
protected:
  void backup()const{}

  int width,height;

public:
  GLFW3Canvas():width(0),height(0){
  }

  void resize( int w,int h ){
    width=w;height=h;
  }

  //MANIPULATORS
  void setFont( BBFont *font ){}
  void setMask( unsigned argb ){}
  void setColor( unsigned argb ){
    int r = (argb >> 16) & 255;
    int g = (argb >> 8) & 255;
    int b = argb & 255;

    glColor3f( r/255.0f,g/255.0f,b/255.0f );
  }
  void setClsColor( unsigned argb ){
    int r = (argb >> 16) & 255;
    int g = (argb >> 8) & 255;
    int b = argb & 255;

    glClearColor( r/255.0f,g/255.0f,b/255.0f,1.0f );
  }
  void setOrigin( int x,int y ){}
  void setHandle( int x,int y ){}
  void setViewport( int x,int y,int w,int h ){
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho( 0.0f,w,h,0.0f,-1.0f,1.0f );
    //
    // glViewport( 0,0,w,h );
  }

  void cls(){
    glClear(GL_COLOR_BUFFER_BIT);
  }
  void plot( int x,int y ){
    glBegin( GL_POINTS );
      glVertex2f( x,y );
    glEnd();
  }
  void line( int x,int y,int x2,int y2 ){
    glBegin( GL_LINES );
      glVertex2f( x,y );
      glVertex2f( x2,y2 );
    glEnd();
  }
  void rect( int x,int y,int w,int h,bool solid ){
    glPointSize( 5.0f );
    // glBegin(GL_POINTS);              // Each set of 4 vertices form a quad
    //    glVertex2f(-0.5f, -0.5f);    // x, y
    //    glVertex2f( 0.5f, -0.5f);
    //    glVertex2f( 0.5f,  0.5f);
    //    glVertex2f(-0.5f,  0.5f);
    // glEnd();
    glBegin( GL_QUADS );              // Each set of 4 vertices form a quad
       glVertex2f( x,y+h );    // x, y
       glVertex2f( x+w,y+h );
       glVertex2f( x+w,y );
       glVertex2f( x,y );
    glEnd();
  }
  void oval( int x,int y,int w,int h,bool solid ){}
  void text( int x,int y,const std::string &t ){}
  void blit( int x,int y,BBCanvas *src,int src_x,int src_y,int src_w,int src_h,bool solid ){}

  bool collide( int x,int y,const BBCanvas *src,int src_x,int src_y,bool solid )const{ return false; }
  bool rect_collide( int x,int y,int rect_x,int rect_y,int rect_w,int rect_h,bool solid )const{ return false; }

  bool lock()const{ return true; }
  void setPixel( int x,int y,unsigned argb ){}
  void setPixelFast( int x,int y,unsigned argb ){}
  void copyPixel( int x,int y,BBCanvas *src,int src_x,int src_y ){}
  void copyPixelFast( int x,int y,BBCanvas *src,int src_x,int src_y ){}
  unsigned getPixel( int x,int y )const{ return 0; }
  unsigned getPixelFast( int x,int y )const{ return 0; }
  void unlock()const{}

  void setCubeMode( int mode ){}
  void setCubeFace( int face ){}

  //ACCESSORS
  int getWidth()const{ return width; }
  int getHeight()const{ return height; }
  int getDepth()const{ return 0; }
  int getFlags()const{ return 0; }
  int cubeMode()const{ return 0; }
  void getOrigin( int *x,int *y )const{}
  void getHandle( int *x,int *y )const{}
  void getViewport( int *x,int *y,int *w,int *h )const{
    *x=0;*y=0;*w=getWidth();*h=getHeight();
  }
  unsigned getMask()const{ return 0; }
  unsigned getColor()const{ return 0; }
  unsigned getClsColor()const{ return 0; }

};

class GLFW3DefaultCanvas : public GLFW3Canvas{
protected:
  GLFWwindow *wnd;
public:
  GLFW3DefaultCanvas( GLFWwindow *wnd ):wnd(wnd){
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
};

class GLFW3Graphics : public B2DGraphics{
protected:
  GLFWwindow *wnd;

public:
  GLFW3Graphics( GLFWwindow *wnd ):wnd(wnd){
    front_canvas=d_new GLFW3DefaultCanvas( wnd );
    back_canvas=d_new GLFW3DefaultCanvas( wnd );
  }

  ~GLFW3Graphics(){
    delete front_canvas;
    delete back_canvas;
  }

  void resize( int w,int h ){
    ((GLFW3Canvas*)front_canvas)->resize( w,h );
    ((GLFW3Canvas*)back_canvas)->resize( w,h );
  }

  // graphics
  void backup(){}
  bool restore(){ return true; }

  //MANIPULATORS
  void vwait(){}

  //SPECIAL!
  void copy( BBCanvas *dest,int dx,int dy,int dw,int dh,BBCanvas *src,int sx,int sy,int sw,int sh ){}

  //NEW! Gamma control!
  void setGamma( int r,int g,int b,float dr,float dg,float db ){}
  void getGamma( int r,int g,int b,float *dr,float *dg,float *db ){}
  void updateGamma( bool calibrate ){}

  //ACCESSORS
  int getWidth()const{ return front_canvas->getWidth(); }
  int getHeight()const{ return front_canvas->getHeight(); }
  int getDepth()const{ return 0; }
  int getScanLine()const{ return 0; }
  int getAvailVidmem()const{ return 0; }
  int getTotalVidmem()const{ return 0; }

  BBFont *getDefaultFont()const{ return 0; }

  //OBJECTS
  BBCanvas *createCanvas( int width,int height,int flags ){
    BBCanvas *canvas=d_new GLFW3Canvas();
    canvas_set.insert( canvas );
    return canvas;
  }
  BBCanvas *loadCanvas( const std::string &file,int flags ){ return 0; }

  // b2dgraphics
  BBMovie *openMovie( const std::string &file,int flags ){ return 0; }
  BBMovie *verifyMovie( BBMovie *movie ){ return 0; }
  void closeMovie( BBMovie *movie ){}

  BBFont *loadFont( const std::string &font,int height,int flags ){ return 0; }
  BBFont *verifyFont( BBFont *font ){ return 0; }
  void freeFont( BBFont *font ){}
};

BBGraphics *GLFW3Runtime::openGraphics( int w,int h,int d,int driver,int flags ){
  if( graphics ) return 0;

  if( (graphics=d_new GLFW3Graphics( wnd )) ){
    glfwSetWindowSize( wnd,w,h );
    glfwShowWindow( wnd );
    glfwMakeContextCurrent( wnd );
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
  glfwSwapBuffers( wnd );
}

void GLFW3Runtime::asyncStop(){
}

void GLFW3Runtime::asyncRun(){
}

void GLFW3Runtime::asyncEnd(){
}

bool GLFW3Runtime::idle(){
  return true;
}

void *GLFW3Runtime::window(){
  return wnd;
}

void GLFW3Runtime::moveMouse( int x,int y ){
}

void GLFW3Runtime::setPointerVisible( bool vis ){
}

void GLFW3Runtime::_onResize( GLFWwindow *w,int width,int height ){
  GLFW3Runtime *rt=runtimes.at( w );
  rt->resize( width,height );
}

void GLFW3Runtime::resize( int width,int height ){
  if( graphics ) graphics->resize( width,height );
}

BBMODULE_CREATE( runtime_glfw3 ){
  return true;
}

BBMODULE_DESTROY( runtime_glfw3 ){
  return true;
}
