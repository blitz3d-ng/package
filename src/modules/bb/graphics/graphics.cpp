
#include "../stdutil/stdutil.h"
#include "graphics.h"
#include <bb/runtime/runtime.h>
#include <bb/graphics/graphics.h>
#include <bb/blitz2d/blitz2d.h>

#include <fstream>
#include <vector>
#include <set>
using namespace std;

#include <math.h>

void blitz2d_open();
void blitz2d_reset();
void blitz2d_close();
void blitz3d_open( int w,int h,float d );
void blitz3d_close();

BBContextDriver *bbContextDriver;
BBGraphics *gx_graphics;
BBCanvas *gx_canvas;

struct GfxMode{
	int w,h,d,caps;
};

static int gx_driver;	//current graphics driver

extern int curs_x,curs_y;
extern BBFont *curr_font;
extern unsigned curr_color;
extern unsigned curr_clsColor;

static vector<GfxMode> gfx_modes;

BBGraphics::BBGraphics():front_canvas(0),back_canvas(0){
}

BBGraphics::~BBGraphics(){
}

BBCanvas *BBGraphics::getFrontCanvas()const{
	return front_canvas;
}

BBCanvas *BBGraphics::getBackCanvas()const{
	return back_canvas;
}

BBCanvas *BBGraphics::verifyCanvas( BBCanvas *c ){
	return canvas_set.count( c ) || c==front_canvas || c==back_canvas ? c : 0;
}

void BBGraphics::freeCanvas( BBCanvas *c ){
	if( canvas_set.erase( c ) ) delete c;
}

static inline void debugCanvas( BBCanvas *c ){
	if( bb_env.debug ){
		if( !gx_graphics->verifyCanvas( c ) ) RTEX( "Buffer does not exist" );
	}
}

static inline void debugDriver( int n ){
	if( bb_env.debug ){
		if( n<1 || n>bbContextDriver->numGraphicsDrivers() ){
			RTEX( "Illegal graphics driver index" );
		}
	}
}

static inline void debugMode( int n ){
	if( bb_env.debug ){
		if( n<1 || n>gfx_modes.size() ){
			RTEX( "Illegal graphics mode index" );
		}
	}
}

static void freeGraphics(){
	blitz3d_close();
	blitz2d_close();
	gx_canvas=0;
	bbContextDriver->closeGraphics( gx_graphics );
}

BBContextDriver::BBContextDriver():graphics(0){
}

BBContextDriver::~BBContextDriver(){
}

bool BBContextDriver::graphicsOpened(){
	return !!graphics;
}

bb_int_t BBCALL bbCountGfxDrivers(){
	return bbContextDriver->numGraphicsDrivers();
}

BBStr *	BBCALL bbGfxDriverName( bb_int_t n ){
	debugDriver( n );
	string t;int caps;
	bbContextDriver->graphicsDriverInfo( n-1,&t,&caps );
	return d_new BBStr( t );
}

void  BBCALL bbSetGfxDriver( bb_int_t n ){
	debugDriver( n );
	gfx_modes.clear();
	gx_driver=n-1;
}

bb_int_t BBCALL bbCountGfxModes(){
	gfx_modes.clear();
	int n=bbContextDriver->numGraphicsModes( gx_driver );
	for( int k=0;k<n;++k ){
		GfxMode m;
		bbContextDriver->graphicsModeInfo( gx_driver,k,&m.w,&m.h,&m.d,&m.caps );
		gfx_modes.push_back( m );
	}
	return gfx_modes.size();
}

bb_int_t BBCALL bbGfxModeWidth( bb_int_t n ){
	debugMode( n );
	return gfx_modes[n-1].w;
}

bb_int_t BBCALL bbGfxModeHeight( bb_int_t n ){
	debugMode( n );
	return gfx_modes[n-1].h;
}

bb_int_t BBCALL bbGfxModeDepth( bb_int_t n ){
	debugMode( n );
	return gfx_modes[n-1].d;
}

static int modeExists( int w,int h,int d,bool bb3d ){
	int cnt=bbContextDriver->numGraphicsModes( gx_driver );
	for( int k=0;k<cnt;++k ){
		int tw,th,td,tc;
		bbContextDriver->graphicsModeInfo( gx_driver,k,&tw,&th,&td,&tc );
		if( bb3d && !(tc&BBContextDriver::GFXMODECAPS_3D) ) continue;
		if( w==tw && h==th && d==td ) return 1;
	}
	return 0;
}

bb_int_t BBCALL bbGfxModeExists( bb_int_t w,bb_int_t h,bb_int_t d ){
	return modeExists( w,h,d,false );
}

#ifdef PRO
bb_int_t BBCALL bbGfxDriver3D( bb_int_t n ){
	debugDriver( n );
	string t;int caps;
	bbContextDriver->graphicsDriverInfo( n-1,&t,&caps );
	return (caps & BBContextDriver::GFXMODECAPS_3D) ? 1 : 0;
}

bb_int_t BBCALL bbCountGfxModes3D(){
	gfx_modes.clear();
	int n=bbContextDriver->numGraphicsModes( gx_driver );
	for( int k=0;k<n;++k ){
		GfxMode m;
		bbContextDriver->graphicsModeInfo( gx_driver,k,&m.w,&m.h,&m.d,&m.caps );
		if( m.caps & BBContextDriver::GFXMODECAPS_3D) gfx_modes.push_back( m );
	}
	return gfx_modes.size();
}

bb_int_t BBCALL bbGfxMode3DExists( bb_int_t w,bb_int_t h,bb_int_t d ){
	return modeExists( w,h,d,true );
}

bb_int_t BBCALL bbGfxMode3D( bb_int_t n ){
	debugMode( n );
	return gfx_modes[n-1].caps & BBContextDriver::GFXMODECAPS_3D ? 1 :0;
}

bb_int_t BBCALL bbWindowed3D(){
	int tc;
	bbContextDriver->windowedModeInfo( &tc );
	return (tc & BBContextDriver::GFXMODECAPS_3D) ? 1 : 0;
}
#endif

bb_int_t BBCALL bbTotalVidMem(){
	return gx_graphics->getTotalVidmem();
}

bb_int_t BBCALL bbAvailVidMem(){
	return gx_graphics->getAvailVidmem();
}

void BBCALL bbSetBuffer( BBCanvas *buff ){
	debugCanvas( buff );
	if( gx_canvas ) gx_canvas->unset();
	gx_canvas=buff;
	curs_x=curs_y=0;
	blitz2d_reset();
}

BBCanvas * BBCALL bbGraphicsBuffer(){
	return gx_canvas;
}

void BBCALL bbBufferDirty( BBCanvas *c ){
	debugCanvas( c );
	c->backup();
}

static void graphics( int w,int h,int d,int flags ){
	freeGraphics();
	gx_graphics=bbContextDriver->openGraphics( w,h,d,gx_driver,flags );
	if( !bbRuntimeIdle() ) RTEX( 0 );
	if( !gx_graphics ){
		RTEX( "Unable to set graphics mode" );
	}
	blitz2d_open();
	BBCanvas *buff=(flags & BBGraphics::GRAPHICS_3D) ?
		gx_graphics->getBackCanvas() : gx_graphics->getFrontCanvas();
	bbSetBuffer( buff );
}

void BBCALL bbGraphics( bb_int_t w,bb_int_t h,bb_int_t d,bb_int_t mode ){
	int flags=0;
	switch( mode ){
	case 0:flags|=bb_env.debug ? BBGraphics::GRAPHICS_WINDOWED : 0 ;break;
	case 1:break;
	case 2:flags|=BBGraphics::GRAPHICS_WINDOWED;break;
	case 3:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_SCALED;break;
	case 6:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_AUTOSUSPEND;break;
	case 7:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_SCALED|BBGraphics::GRAPHICS_AUTOSUSPEND;break;
	default:RTEX( "Illegal Graphics mode" );
	}
	graphics( w,h,d,flags );
}

#ifdef PRO

void BBCALL bbGraphics3D( bb_int_t w,bb_int_t h,bb_int_t d,bb_int_t mode ){
	int flags=BBGraphics::GRAPHICS_3D;
	switch( mode ){
	case 0:flags|=(bb_env.debug && bbWindowed3D()) ? BBGraphics::GRAPHICS_WINDOWED : 0 ;break;
	case 1:break;
	case 2:flags|=BBGraphics::GRAPHICS_WINDOWED;break;
	case 3:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_SCALED;break;
	case 6:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_AUTOSUSPEND;break;
	case 7:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_SCALED|BBGraphics::GRAPHICS_AUTOSUSPEND;break;
	default:RTEX( "Illegal Graphics3D mode" );
	}
	graphics( w,h,d,flags );
	blitz3d_open( w,h,gx_graphics->getDensity() );
}
#endif

bool BBCALL bbDefaultGraphics(){
	freeGraphics();
	gx_graphics=bbContextDriver->openGraphics( 400,300,0,0,BBGraphics::GRAPHICS_WINDOWED );
	if( !bbRuntimeIdle() ) RTEX( 0 );
	if( gx_graphics ){
		blitz2d_open();
		bbSetBuffer( bbFrontBuffer() );
		return true;
	}
	return false;
}

void BBCALL bbEndGraphics(){
	bbContextDriver->closeGraphics( gx_graphics );
	if( !bbDefaultGraphics() ){
		RTEX( "Unable to set graphics mode" );
	}
}

bb_int_t BBCALL bbGraphicsLost(){
	return bbContextDriver->graphicsLost();
}

bb_int_t BBCALL bbGraphicsOpen(){
	return !!bbContextDriver->graphicsOpened();
}

void BBCALL bbSetGamma( bb_int_t r,bb_int_t g,bb_int_t b,bb_float_t dr,bb_float_t dg,bb_float_t db ){
	if( dr<0 ) dr=0;
	else if( dr>255.0f ) dr=255.0f;
	if( dg<0 ) dg=0;
	else if( dg>255.0f ) dg=255.0f;
	if( db<0 ) db=0;
	else if( db>255.0f ) db=255.0f;
	gx_graphics->setGamma( r,g,b,dr,dg,db );
}

void BBCALL bbUpdateGamma( bb_int_t calibrate ){
	gx_graphics->updateGamma( !!calibrate );
}

bb_float_t BBCALL bbGammaRed( bb_int_t n ){
	float dr,dg,db;
	gx_graphics->getGamma( n,n,n,&dr,&dg,&db );
	return dr;
}

bb_float_t BBCALL bbGammaGreen( bb_int_t n ){
	float dr,dg,db;
	gx_graphics->getGamma( n,n,n,&dr,&dg,&db );
	return dg;
}

bb_float_t BBCALL bbGammaBlue( bb_int_t n ){
	float dr,dg,db;
	gx_graphics->getGamma( n,n,n,&dr,&dg,&db );
	return db;
}

BBCanvas * BBCALL bbFrontBuffer(){
	return gx_graphics->getFrontCanvas();
}

BBCanvas * BBCALL bbBackBuffer(){
	return gx_graphics->getBackCanvas();
}

void BBCALL bbLockBuffer( BBCanvas *buff ){
	if( buff ) debugCanvas( buff );
	(buff ? buff : gx_canvas)->lock();
}

void BBCALL bbUnlockBuffer( BBCanvas *buff ){
	if( buff ) debugCanvas( buff );
	(buff ? buff : gx_canvas)->unlock();
}

bb_int_t BBCALL bbReadPixel( bb_int_t x,bb_int_t y,BBCanvas *buff ){
	if( buff ) debugCanvas( buff );
	return (buff ? buff : gx_canvas)->getPixel( x,y );
}

void BBCALL bbWritePixel( bb_int_t x,bb_int_t y,bb_int_t argb,BBCanvas *buff ){
	if( buff ) debugCanvas( buff );
	(buff ? buff : gx_canvas)->setPixel( x,y,argb );
}

bb_int_t BBCALL bbReadPixelFast( bb_int_t x,bb_int_t y,BBCanvas *buff ){
	return (buff ? buff : gx_canvas)->getPixelFast( x,y );
}

void BBCALL bbWritePixelFast( bb_int_t x,bb_int_t y,bb_int_t argb,BBCanvas *buff ){
	(buff ? buff : gx_canvas)->setPixelFast( x,y,argb );
}

void BBCALL bbCopyPixel( bb_int_t src_x,bb_int_t src_y,BBCanvas *src,bb_int_t dest_x,bb_int_t dest_y,BBCanvas *buff ){
	(buff ? buff : gx_canvas)->copyPixel( dest_x,dest_y,src ? src : gx_canvas,src_x,src_y );
}

void BBCALL bbCopyPixelFast( bb_int_t src_x,bb_int_t src_y,BBCanvas *src,bb_int_t dest_x,bb_int_t dest_y,BBCanvas *buff ){
	(buff ? buff : gx_canvas)->copyPixelFast( dest_x,dest_y,src ? src : gx_canvas,src_x,src_y );
}

bb_int_t BBCALL bbScanLine(){
	return gx_graphics->getScanLine();
}

void BBCALL bbVWait( bb_int_t n ){
	gx_graphics->vwait();
	if( !bbRuntimeIdle() ) RTEX( 0 );
}

void BBCALL bbFlip( bb_int_t vwait ){
	bbContextDriver->flip( vwait ? true : false );
	if( !bbRuntimeIdle() ) RTEX( 0 );
}

bb_int_t BBCALL bbGraphicsWidth(){
	return gx_graphics->getLogicalWidth();
}

bb_int_t BBCALL bbGraphicsHeight(){
	return gx_graphics->getLogicalHeight();
}

bb_int_t BBCALL bbGraphicsDepth(){
	return gx_graphics->getDepth();
}

void BBCALL bbCopyRect( bb_int_t sx,bb_int_t sy,bb_int_t w,bb_int_t h,bb_int_t dx,bb_int_t dy,BBCanvas *src,BBCanvas *dest ){
	if( src ) debugCanvas( src );
	else src=gx_canvas;
	if( dest ) debugCanvas( dest );
	else dest=gx_canvas;
	dest->blit( dx,dy,src,sx,sy,w,h,true );
}

BBMODULE_CREATE( graphics ){
	gx_driver=0;

	return true;
}

BBMODULE_DESTROY( graphics ){
	freeGraphics();
	gfx_modes.clear();
	if( gx_graphics ){
		bbContextDriver->closeGraphics( gx_graphics );
		gx_graphics=0;
	}
	return true;
}
