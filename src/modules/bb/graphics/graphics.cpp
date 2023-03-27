
#include "../stdutil/stdutil.h"
#include "graphics.h"
#include <bb/runtime/runtime.h>
#include <bb/system/system.h>
#include <bb/input/input.h>
#include <bb/graphics/graphics.h>

#ifdef WIN32
#include <windows.h>
#endif


#include <fstream>
#include <vector>
#include <set>
using namespace std;

#include <math.h>

//degrees to radians
static const float dtor=0.0174532925199432957692369076848861f;

class BBImage{
public:
	BBImage( const vector<BBCanvas*> &f ):frames(f){
	}
	~BBImage(){
		for( int k=0;k<frames.size();++k ){
			gx_graphics->freeCanvas( frames[k] );
		}
	}
	const vector<BBCanvas*> &getFrames()const{
		return frames;
	}
	void replaceFrame( int n,BBCanvas *c ){
		gx_graphics->freeCanvas( frames[n] );
		frames[n]=c;
	}
private:
	vector<BBCanvas*> frames;
};

extern FT_Library ft;
static bool filter=true;
static bool auto_dirty=true;
static bool auto_midhandle=false;
static set<BBImage*> image_set;
/*static*/ int curs_x=0,curs_y=0;
static BBCanvas *p_canvas=0;
/*static*/ BBFont *curr_font=0;
/*static*/ unsigned curr_color=0;
/*static*/ unsigned curr_clsColor=0;

static inline void debugImage( BBImage *i,int frame=0 ){
	if( bb_env.debug ){
		if( !image_set.count(i) ) RTEX( "Image does not exist" );
		if( frame>=i->getFrames().size() ) RTEX( "Image frame out of range" );
	}
}

static inline void debugFont( BBFont *f ){
	if( bb_env.debug ){
		if( !gx_graphics->verifyFont( f ) ) RTEX( "Font does not exist" );
	}
}

static inline void debugCanvas( BBCanvas *c ){
	if( bb_env.debug ){
		if( !gx_graphics->verifyCanvas( c ) ) RTEX( "Buffer does not exist" );
	}
}

void blitz2d_open();
void blitz2d_reset();
void blitz2d_close();
void blitz3d_open( BBGraphics *graphics );
void blitz3d_close();

std::vector<BBCreateContextDriver*> bbContextDrivers;
BBContextDriver *bbContextDriver=0;
BBGraphics *gx_graphics=0;
BBCanvas *gx_canvas=0;

struct GfxMode{
	int w,h,d,caps;
};

static int gx_driver=0;	//current graphics driver

static vector<GfxMode> gfx_modes;

BBGraphics::BBGraphics():front_canvas(0),back_canvas(0){
}

BBGraphics::~BBGraphics(){
	while( movie_set.size() ) closeMovie( *movie_set.begin() );
	while( font_set.size() ) freeFont( *font_set.begin() );
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
	if( gx_graphics ){
		bbContextDriver->closeGraphics( gx_graphics );
		gx_graphics=0;
	}
}

BBContextDriver::BBContextDriver():graphics(0){
}

BBContextDriver::~BBContextDriver(){
}

bool BBContextDriver::graphicsOpened(){
	return !!graphics;
}

int BBContextDriver::change( const string &name ){
	// TODO: not supporting switching right now
	if( bbContextDriver ) return 0;

	bbContextDriver=0;
	for( auto fn:bbContextDrivers ){
		bbContextDriver=fn( name );
		if( bbContextDriver ) return 1;
	}

	return 0;
}

BBLIB bb_int_t BBCALL bbCountRenderers(){
	return bbContextDrivers.size();
}

BBLIB BBStr * BBCALL bbRendererName( bb_int_t renderer ){
	return d_new BBStr( "<not implemented>" );
}

BBLIB bb_int_t BBCALL bbSetRenderer( BBStr *name ){
	string s=*name;delete name;
	return BBContextDriver::change( s );
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
	blitz3d_open( gx_graphics );
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

#define RED(_X_) ( ((_X_)>>16) & 0xff )
#define GRN(_X_) ( ((_X_)>>8) & 0xff )
#define BLU(_X_) ( (_X_) & 0xff )

static int getPixel( BBCanvas *c,float x,float y ){
	debugCanvas( c );

	x-=.5f;y-=.5f;
	float fx=floor(x),fy=floor(y);
	int ix=fx,iy=fy;fx=x-fx;fy=y-fy;

	int tl=c->getPixel( ix,iy );
	int tr=c->getPixel( ix+1,iy );
	int br=c->getPixel( ix+1,iy+1 );
	int bl=c->getPixel( ix,iy+1 );

	float w1=(1-fx)*(1-fy),w2=fx*(1-fy),w3=(1-fx)*fy,w4=fx*fy;

	float r=RED(tl)*w1+RED(tr)*w2+RED(bl)*w3+RED(br)*w4;
	float g=GRN(tl)*w1+GRN(tr)*w2+GRN(bl)*w3+GRN(br)*w4;
	float b=BLU(tl)*w1+BLU(tr)*w2+BLU(bl)*w3+BLU(br)*w4;

	return (int(r+.5f)<<16)|(int(g+.5f)<<8)|int(b+.5f);
}

struct vec2{ float x,y; };

static vec2 vrot( float m[2][2],const vec2 &v ){
	vec2 t;t.x=m[0][0]*v.x+m[0][1]*v.y;t.y=m[1][0]*v.x+m[1][1]*v.y;
	return t;
}

static float vmin( float a,float b,float c,float d ){
	float t=a;if( b<t ) t=b;if( c<t ) t=c;if( d<t ) t=d;return t;
}

static float vmax( float a,float b,float c,float d ){
	float t=a;if( b>t ) t=b;if( c>t ) t=c;if( d>t ) t=d;return t;
}

static BBCanvas *tformCanvas( BBCanvas *c,float m[2][2],int x_handle,int y_handle ){

	vec2 v,v0,v1,v2,v3;
	float i[2][2];
	float dt=1.0f/(m[0][0]*m[1][1]-m[1][0]*m[0][1]);
	i[0][0]=dt*m[1][1];i[1][0]=-dt*m[1][0];
	i[0][1]=-dt*m[0][1];i[1][1]=dt*m[0][0];

	float ox=x_handle,oy=y_handle;
	v0.x=-ox;v0.y=-oy;	//tl
	v1.x=c->getWidth()-ox;v1.y=-oy;	//tr
	v2.x=c->getWidth()-ox;v2.y=c->getHeight()-oy;	//br
	v3.x=-ox;v3.y=c->getHeight()-oy;	//bl
	v0=vrot(m,v0);v1=vrot(m,v1);v2=vrot(m,v2);v3=vrot(m,v3);
	float minx=floor( vmin( v0.x,v1.x,v2.x,v3.x ) );
	float miny=floor( vmin( v0.y,v1.y,v2.y,v3.y ) );
	float maxx=ceil( vmax( v0.x,v1.x,v2.x,v3.x ) );
	float maxy=ceil( vmax( v0.y,v1.y,v2.y,v3.y ) );
	int iw=maxx-minx,ih=maxy-miny;

	BBCanvas *t=gx_graphics->createCanvas( iw,ih,0 );
	t->setHandle( -minx,-miny );
	t->setMask( c->getMask() );

	c->lock();
	t->lock();

	v.y=miny+.5f;
	for( int y=0;y<ih;++v.y,++y ){
		v.x=minx+.5f;
		for( int x=0;x<iw;++v.x,++x ){
			vec2 q=vrot(i,v);
			unsigned rgb=filter ? getPixel( c,q.x+ox,q.y+oy ) : c->getPixel( floor(q.x+ox),floor(q.y+oy) );
			t->setPixel( x,y,rgb );
		}
	}

	t->unlock();
	c->unlock();

	return t;
}

static bool saveCanvas( BBCanvas *c,const string &f ){
#ifndef WIN32 // FIXME: port to posix
	return false;
#else
	std::ofstream out( f.c_str(),std::ios::binary );
	if( !out.good() ) return false;

	int tempsize=(c->getWidth()*3+3)&~3;

	BITMAPFILEHEADER bf;
	memset( &bf,0,sizeof(bf) );
	bf.bfType='MB';
	bf.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+tempsize*c->getHeight();
	bf.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	BITMAPINFOHEADER bi;memset( &bi,0,sizeof(bi) );
	bi.biSize=sizeof(bi);
	bi.biWidth=c->getWidth();
	bi.biHeight=c->getHeight();
	bi.biPlanes=1;
	bi.biBitCount=24;
	out.write( (char*)&bf,sizeof(bf) );
	out.write( (char*)&bi,sizeof(bi) );

	unsigned char *temp=d_new unsigned char[ tempsize ];
	memset( temp,0,tempsize );

	c->lock();
	for( int y=c->getHeight()-1;y>=0;--y ){
		unsigned char *dest=temp;
		for( int x=0;x<c->getWidth();++x ){
			unsigned rgb=c->getPixelFast( x,y );
			*dest++=rgb&0xff;
			*dest++=(rgb>>8)&0xff;
			*dest++=(rgb>>16)&0xff;
		}
		out.write( (char*)temp,tempsize );
	}
	c->unlock();

	delete [] temp;

	return out.good();
#endif
}

bb_int_t BBCALL bbLoadBuffer( BBCanvas *c,BBStr *str ){
	debugCanvas( c );
	string s=*str;delete str;
	BBCanvas *t=gx_graphics->loadCanvas( s,0 );
	if( !t ) return 0;
	float m[2][2];
	m[0][0]=(float)c->getWidth()/(float)t->getWidth();
	m[1][1]=(float)c->getHeight()/(float)t->getHeight();
	m[1][0]=m[0][1]=0;
	BBCanvas *p=tformCanvas( t,m,0,0 );
	gx_graphics->freeCanvas( t );
	int ox,oy;
	c->getOrigin( &ox,&oy );c->setOrigin( 0,0 );
	c->blit( 0,0,p,0,0,p->getWidth(),p->getHeight(),true );
	gx_graphics->freeCanvas( p );
	return 1;
}

bb_int_t BBCALL bbSaveBuffer( BBCanvas *c,BBStr *str ){
	debugCanvas( c );
	string t=*str;delete str;
	return saveCanvas( c,t ) ? 1 : 0;
}

void BBCALL bbOrigin( bb_int_t x,bb_int_t y ){
	gx_canvas->setOrigin( x,y );
}

void BBCALL bbViewport( bb_int_t x,bb_int_t y,bb_int_t w,bb_int_t h ){
	gx_canvas->setViewport( x,y,w,h );
}

void BBCALL bbColor( bb_int_t r,bb_int_t g,bb_int_t b ){
	gx_canvas->setColor( curr_color=(r<<16)|(g<<8)|b );
}

void BBCALL bbGetColor( bb_int_t x,bb_int_t y ){
	gx_canvas->setColor( curr_color=gx_canvas->getPixel( x,y ) );
}

bb_int_t BBCALL bbColorRed(){
	return (gx_canvas->getColor()>>16)&0xff;
}

bb_int_t BBCALL bbColorGreen(){
	return (gx_canvas->getColor()>>8)&0xff;
}

bb_int_t BBCALL bbColorBlue(){
	return gx_canvas->getColor()&0xff;
}

void BBCALL bbClsColor( bb_int_t r,bb_int_t g,bb_int_t b ){
	gx_canvas->setClsColor( curr_clsColor=(r<<16)|(g<<8)|b );
}

void BBCALL bbCls(){
	gx_canvas->cls();
}

void BBCALL bbPlot( bb_int_t x,bb_int_t y ){
	gx_canvas->plot( x,y );
}

void BBCALL bbLine( bb_int_t x1,bb_int_t y1,bb_int_t x2,bb_int_t y2 ){
	gx_canvas->line( x1,y1,x2,y2 );
}

void BBCALL bbRect( bb_int_t x,bb_int_t y,bb_int_t w,bb_int_t h,bb_int_t solid ){
	gx_canvas->rect( x,y,w,h,solid ? true : false );
}

void BBCALL bbOval( bb_int_t x,bb_int_t y,bb_int_t w,bb_int_t h,bb_int_t solid ){
	gx_canvas->oval( x,y,w,h,solid ? true : false );
}

void BBCALL bbText( bb_int_t x,bb_int_t y,BBStr *str,bb_int_t centre_x,bb_int_t centre_y ){
	debugFont( curr_font );
	if( centre_x ) x-=curr_font->getWidth( *str )/2;
	if( centre_y ) y-=curr_font->getHeight()/2;
	gx_canvas->text( x,y,*str );
	delete str;
}

BBFont * BBCALL bbLoadFont( BBStr *name,bb_int_t height,bb_int_t bold,bb_int_t italic,bb_int_t underline ){
	int flags=
		(bold ? BBFont::FONT_BOLD : 0 ) |
		(italic ? BBFont::FONT_ITALIC : 0 ) |
		(underline ? BBFont::FONT_UNDERLINE : 0 );
	BBFont *font=gx_graphics->loadFont( *name,height,flags );
	delete name;
	return font;
}

void BBCALL bbFreeFont( BBFont *f ){
	debugFont( f );
	if( f==curr_font ) bbSetFont( gx_graphics->getDefaultFont() );
	gx_graphics->freeFont( f );
}

void BBCALL bbSetFont( BBFont *f ){
	debugFont( f );
	gx_canvas->setFont( curr_font=f );
}

bb_int_t BBCALL bbFontWidth(){
	debugFont( curr_font );
	return curr_font->getWidth();
}

bb_int_t BBCALL bbFontHeight(){
	debugFont( curr_font );
	return curr_font->getHeight();
}

bb_int_t BBCALL bbStringWidth( BBStr *str ){
	debugFont( curr_font );
	string t=*str;delete str;
	return curr_font->getWidth( t );
}

bb_int_t BBCALL bbStringHeight( BBStr *str ){
	debugFont( curr_font );
	delete str;
	return curr_font->getHeight();
}

BBImage * BBCALL bbLoadImage( BBStr *s ){
	string t=*s;delete s;
	BBCanvas *c=gx_graphics->loadCanvas( t,0 );
	if( !c ) return 0;
	if( auto_dirty ) c->backup();
	if( auto_midhandle ) c->setHandle( c->getWidth()/2,c->getHeight()/2 );
	vector<BBCanvas*> frames;
	frames.push_back( c );
	BBImage *i=d_new BBImage( frames );
	image_set.insert( i );
	return i;
}

BBImage * BBCALL bbLoadAnimImage( BBStr *s,bb_int_t w,bb_int_t h,bb_int_t first,bb_int_t cnt ){

	string t=*s;delete s;

	if( cnt<1 ) RTEX( "Illegal frame count" );
	if( first<0 ) RTEX( "Illegal first frame" );

	BBCanvas *pic=gx_graphics->loadCanvas( t,BBCanvas::CANVAS_NONDISPLAY );
	if( !pic ) return 0;

	//frames per row, per picture
	int fpr=pic->getWidth()/w;
	int fpp=pic->getHeight()/h*fpr;
	if( first+cnt>fpp ){
		gx_graphics->freeCanvas( pic );
		RTEX( "Not enough frames in bitmap" );
	}

	//x,y of first frame...
	vector<BBCanvas*> frames;
	int src_x=first%fpr*w,src_y=first/fpr*h;

	for( int k=0;k<cnt;++k ){
		BBCanvas *c=gx_graphics->createCanvas( w,h,0 );
		if( !c ){
			for( --k;k>=0;--k ) gx_graphics->freeCanvas( frames[k] );
			gx_graphics->freeCanvas( pic );return 0;
		}
		c->blit( 0,0,pic,src_x,src_y,w,h,true );
		if( auto_dirty ) c->backup();
		if( auto_midhandle ) c->setHandle( c->getWidth()/2,c->getHeight()/2 );
		frames.push_back( c );
		src_x+=w;if( src_x+w>pic->getWidth() ){ src_x=0;src_y+=h; }
	}
	gx_graphics->freeCanvas( pic );
	BBImage *i=d_new BBImage( frames );
	image_set.insert( i );
	return i;
}

BBImage * BBCALL bbCopyImage( BBImage *i ){
	debugImage( i );
	vector<BBCanvas*> frames;
	const vector<BBCanvas*> &f=i->getFrames();
	for( int k=0;k<f.size();++k ){
		BBCanvas *t=f[k];
		BBCanvas *c=gx_graphics->createCanvas( t->getWidth(),t->getHeight(),0 );
		if( !c ){
			for( --k;k>=0;--k ) gx_graphics->freeCanvas( frames[k] );
			return 0;
		}
		int x,y;
		t->getHandle( &x,&y );
		t->setHandle( 0,0 );
		c->blit( 0,0,t,0,0,t->getWidth(),t->getHeight(),true );
		if( auto_dirty ) c->backup();
		t->setHandle( x,y );
		c->setHandle( x,y );
		c->setMask( t->getMask() );
		frames.push_back( c );
	}
	BBImage *t=d_new BBImage( frames );
	image_set.insert( t );
	return t;
}

BBImage * BBCALL bbCreateImage( bb_int_t w,bb_int_t h,bb_int_t n ){
	vector<BBCanvas*> frames;
	for( int k=0;k<n;++k ){
		BBCanvas *c=gx_graphics->createCanvas( w,h,0 );
		if( !c ){
			for( --k;k>=0;--k ) gx_graphics->freeCanvas( frames[k] );
			return 0;
		}
		if( auto_dirty ) c->backup();
		if( auto_midhandle ) c->setHandle( c->getWidth()/2,c->getHeight()/2 );
		frames.push_back( c );
	}
	BBImage *i=d_new BBImage( frames );
	image_set.insert( i );
	return i;
}

void BBCALL bbFreeImage( BBImage *i ){
	if( !image_set.erase(i) ) return;
	const vector<BBCanvas*> &f=i->getFrames();
	for( int k=0;k<f.size();++k ){
		if( f[k]==gx_canvas ){
			bbSetBuffer( gx_graphics->getFrontCanvas() );
			break;
		}
	}
	delete i;
}

bb_int_t BBCALL bbSaveImage( BBImage *i,BBStr *str,bb_int_t n ){
	debugImage( i,n );
	string t=*str;delete str;
	BBCanvas *c=i->getFrames()[n];
	return saveCanvas( c,t ) ? 1 : 0;
}

void BBCALL bbGrabImage( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t n ){
	debugImage( i,n );
	BBCanvas *c=i->getFrames()[n];
	int src_ox,src_oy,dst_hx,dst_hy;
	gx_canvas->getOrigin( &src_ox,&src_oy );
	c->getHandle( &dst_hx,&dst_hy );
	x+=src_ox-dst_hx;y+=src_oy-dst_hy;
	c->setViewport( 0,0,c->getWidth(),c->getHeight() );
	c->blit( 0,0,gx_canvas,x,y,c->getWidth(),c->getHeight(),true );
	if( auto_dirty ) c->backup();
}

BBCanvas * BBCALL bbImageBuffer( BBImage *i,bb_int_t n ){
	debugImage( i,n );
	return i->getFrames()[n];
}

void BBCALL bbDrawImage( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t frame ){
	debugImage( i,frame );
	BBCanvas *c=i->getFrames()[frame];
	gx_canvas->image( c,x,y,false );
}

void BBCALL bbDrawBlock( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t frame ){
	debugImage( i,frame );
	BBCanvas *c=i->getFrames()[frame];
	gx_canvas->image( c,x,y,true );
}

static void tile( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t frame,bool solid ){
	BBCanvas *c=i->getFrames()[frame];

	int hx,hy;
	c->getHandle( &hx,&hy );
	int w=c->getWidth(),h=c->getHeight();

	int ox,oy,vp_x,vp_y,vp_w,vp_h;
	gx_canvas->getOrigin( &ox,&oy );
	gx_canvas->getViewport( &vp_x,&vp_y,&vp_w,&vp_h );
	int dx=vp_x-ox+hx;
	int dy=vp_y-oy+hy;
	x-=dx;
	y-=dy;
	dx+=(x>=0?x%w:w-(-x%w));
	dy+=(y>=0?y%h:h-(-y%h));

	for( y=-h;y<vp_h;y+=h ){
		for( x=-w;x<vp_w;x+=w ){
			gx_canvas->blit( x+dx,y+dy,c,0,0,w,h,solid );
		}
	}
}

void BBCALL bbTileImage( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t frame ){
	debugImage( i,frame );
	tile( i,x,y,frame,false );
}

void BBCALL bbTileBlock( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t frame ){
	debugImage( i,frame );
	tile( i,x,y,frame,true );
}

void BBCALL bbDrawImageRect( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t r_x,bb_int_t r_y,bb_int_t r_w,bb_int_t r_h,bb_int_t frame ){
	debugImage( i,frame );
	BBCanvas *c=i->getFrames()[frame];
	gx_canvas->blit( x,y,c,r_x,r_y,r_w,r_h,false );
}

void BBCALL bbDrawBlockRect( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t r_x,bb_int_t r_y,bb_int_t r_w,bb_int_t r_h,bb_int_t frame ){
	debugImage( i,frame );
	BBCanvas *c=i->getFrames()[frame];
	gx_canvas->blit( x,y,c,r_x,r_y,r_w,r_h,true );
}

void BBCALL bbMaskImage( BBImage *i,bb_int_t r,bb_int_t g,bb_int_t b ){
	debugImage( i );
	unsigned argb=(r<<16)|(g<<8)|b;
	const vector<BBCanvas*> &f=i->getFrames();
	for( int k=0;k<f.size();++k ) f[k]->setMask( argb );
}

void BBCALL bbHandleImage( BBImage *i,bb_int_t x,bb_int_t y ){
	debugImage( i );
	const vector<BBCanvas*> &f=i->getFrames();
	for( int k=0;k<f.size();++k ) f[k]->setHandle( x,y );
}

void BBCALL bbMidHandle( BBImage *i ){
	debugImage( i );
	const vector<BBCanvas*> &f=i->getFrames();
	for( int k=0;k<f.size();++k ) f[k]->setHandle( f[k]->getWidth()/2,f[k]->getHeight()/2 );
}

void BBCALL bbAutoMidHandle( bb_int_t enable ){
	auto_midhandle=enable ? true : false;
}

bb_int_t BBCALL bbImageWidth( BBImage *i ){
	debugImage( i );
	return i->getFrames()[0]->getWidth();
}

bb_int_t BBCALL bbImageHeight( BBImage *i ){
	debugImage( i );
	return i->getFrames()[0]->getHeight();
}

bb_int_t BBCALL bbImageXHandle( BBImage *i ){
	debugImage( i );
	int x,y;
	i->getFrames()[0]->getHandle( &x,&y );
	return x;
}

bb_int_t BBCALL bbImageYHandle( BBImage *i ){
	debugImage( i );
	int x,y;
	i->getFrames()[0]->getHandle( &x,&y );
	return y;
}

bb_int_t BBCALL bbImagesOverlap( BBImage *i1,bb_int_t x1,bb_int_t y1,BBImage *i2,bb_int_t x2,bb_int_t y2 ){
	debugImage( i1 );
	debugImage( i2 );
	BBCanvas *c1=i1->getFrames()[0];
	BBCanvas *c2=i2->getFrames()[0];
	return c1->collide( x1,y1,c2,x2,y2,true );
}

bb_int_t BBCALL bbImagesCollide( BBImage *i1,bb_int_t x1,bb_int_t y1,bb_int_t f1,BBImage *i2,bb_int_t x2,bb_int_t y2,bb_int_t f2 ){
	debugImage( i1,f1 );
	debugImage( i2,f2 );
	BBCanvas *c1=i1->getFrames()[f1];
	BBCanvas *c2=i2->getFrames()[f2];
	return c1->collide( x1,y1,c2,x2,y2,false );
}

bb_int_t BBCALL bbRectsOverlap( bb_int_t x1,bb_int_t y1,bb_int_t w1,bb_int_t h1,bb_int_t x2,bb_int_t y2,bb_int_t w2,bb_int_t h2 ){
	if( x1+w1<=x2 || x1>=x2+w2 || y1+h1<=y2 || y1>=y2+h2 ) return 0;
	return 1;
}

bb_int_t BBCALL bbImageRectOverlap( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t x2,bb_int_t y2,bb_int_t w2,bb_int_t h2 ){
	debugImage( i );
	BBCanvas *c=i->getFrames()[0];
	return c->rect_collide( x,y,x2,y2,w2,h2,true );
}

bb_int_t BBCALL bbImageRectCollide( BBImage *i,bb_int_t x,bb_int_t y,bb_int_t f,bb_int_t x2,bb_int_t y2,bb_int_t w2,bb_int_t h2 ){
	debugImage( i,f );
	BBCanvas *c=i->getFrames()[f];
	return c->rect_collide( x,y,x2,y2,w2,h2,false );
}

void BBCALL bbTFormImage( BBImage *i,bb_float_t a,bb_float_t b,bb_float_t c,bb_float_t d ){
	debugImage( i );
	const vector<BBCanvas*> &f=i->getFrames();
	int k;
	for( k=0;k<f.size();++k ){
		if( f[k]==gx_canvas ){
			bbSetBuffer( gx_graphics->getFrontCanvas() );
			break;
		}
	}
	float m[2][2];
	m[0][0]=a;m[1][0]=b;m[0][1]=c;m[1][1]=d;
	for( k=0;k<f.size();++k ){
		BBCanvas *c=f[k];
		int hx,hy;c->getHandle( &hx,&hy );
		BBCanvas *t=tformCanvas( c,m,hx,hy );
		i->replaceFrame( k,t );
		t->backup();
	}
}

void BBCALL bbScaleImage( BBImage *i,bb_float_t w,bb_float_t h ){
	debugImage( i );
	bbTFormImage( i,w,0,0,h );
}

void BBCALL bbResizeImage( BBImage *i,bb_float_t w,bb_float_t h ){
	debugImage( i );
	BBCanvas *c=i->getFrames()[0];
	bbTFormImage( i,w/(float)c->getWidth(),0,0,h/(float)c->getHeight() );
}

void BBCALL bbRotateImage( BBImage *i,bb_float_t d ){
	debugImage( i );
	d*=-dtor;
	bbTFormImage( i,cos(d),-sin(d),sin(d),cos(d) );
}

void BBCALL bbTFormFilter( bb_int_t enable ){
	filter=enable ? true : false;
}

static int p_ox,p_oy,p_hx,p_hy,p_vpx,p_vpy,p_vpw,p_vph;

static BBCanvas *startPrinting(){

	BBCanvas *c=gx_graphics->getFrontCanvas();

	c->lock();
	c->unlock();

	c->getOrigin( &p_ox,&p_oy );
	c->getHandle( &p_hx,&p_hy );
	c->getViewport( &p_vpx,&p_vpy,&p_vpw,&p_vph );

	c->setOrigin( 0,0 );
	c->setHandle( 0,0 );
	c->setViewport( 0,0,c->getWidth(),c->getHeight() );
	if( c!=gx_canvas ){
		c->setFont( curr_font );
		c->setColor( curr_color );
	}

	int dy=curs_y+curr_font->getHeight()-c->getHeight();
	if( dy>0 ){
		curs_y=c->getHeight()-curr_font->getHeight();
		c->blit( 0,0,c,0,dy,c->getWidth(),c->getHeight()-dy,true );
		c->setColor( curr_clsColor );
		c->rect( 0,c->getHeight()-dy,c->getWidth(),dy,true );
		c->setColor( curr_color );
	}
	return c;
}

static void endPrinting( BBCanvas *c ){
	c->setViewport( p_vpx,p_vpy,p_vpw,p_vph );
	c->setHandle( p_hx,p_hy );
	c->setOrigin( p_ox,p_oy );
	if( c==gx_canvas ) c->setColor( curr_color );
	if( !bbRuntimeIdle() ) RTEX( 0 );
}

void BBCALL bbWrite( BBStr *str ){
	if( !curr_font ) return;
	BBCanvas *c=startPrinting();
	c->text( curs_x,curs_y,*str );
	curs_x+=curr_font->getWidth( *str );
	endPrinting( c );
	delete str;
}

void BBCALL bbPrint( BBStr *str ){
	if( !curr_font ) return;
	BBCanvas *c=startPrinting();
	c->text( curs_x,curs_y,*str );
	curs_x=0;
	curs_y+=curr_font->getHeight();
	endPrinting( c );
	delete str;
}

BBStr * BBCALL bbInput( BBStr *prompt ){
	if( !curr_font ) return d_new BBStr( "" );
	BBCanvas *c=startPrinting();
	string t=*prompt;delete prompt;

	//get temp canvas
	if( !p_canvas || p_canvas->getWidth()<c->getWidth() || p_canvas->getHeight()<curr_font->getHeight()*2 ){
		if( p_canvas ) gx_graphics->freeCanvas( p_canvas );
		p_canvas=gx_graphics->createCanvas( c->getWidth(),curr_font->getHeight()*2,0 );
		if( !p_canvas ){
			endPrinting(c);
			return d_new BBStr();
		}
	}
	//draw prompt
	c->text( curs_x,curs_y,t );
	curs_x+=curr_font->getWidth( t );

	p_canvas->setFont( curr_font );
	p_canvas->setColor( curr_color );
	p_canvas->blit( 0,0,c,0,curs_y,c->getWidth(),curr_font->getHeight(),true );

	string str;
	bool go=true;
	int curs=0,last_key=0,last_time,rep_delay;

	while( go ){

		//render all text
		//calc curs x and width
		int cx=curs_x+curr_font->getWidth( str.substr( 0,curs ) );
		int cw=curr_font->getWidth( curs<str.size() ? str.substr( curs,1 ) : "X" );

		//wait for a key
		int key=0,st=bbMilliSecs(),tc=-1;

		while( bbRuntimeIdle() ){
			int t=bbMilliSecs();
			int n=(t-st)/320;
			if( n!=tc ){
				tc=n;
				if( !(tc&1) ){	//cursor ON
					c->setColor( curr_clsColor^0xffffff );
					c->rect( cx,curs_y,cw,curr_font->getHeight(),true );
					c->setColor( curr_clsColor );
				}else{			//cursor OFF
					c->blit( cx,curs_y,p_canvas,cx,0,cw,curr_font->getHeight(),true );
					c->setColor( curr_color );
				}
				c->text( cx,curs_y,str.substr( curs,1 ) );
			}
			if( (key=bbGetKey( false )) ){
				if( int asc=gx_input->toAscii( key ) ){
					rep_delay=280;
					last_key=key;
					last_time=t;
					key=asc;
					break;
				}
			}
			if( last_key && bbKeyDown( last_key ) ){
				if( t-last_time>rep_delay ){
					if( (key=gx_input->toAscii( last_key )) ){
						last_time+=rep_delay;
						rep_delay=40;
						break;
					}
				}
			}else last_key=0;
			bbDelay( 20 );
		}

		//check the key
		switch( key ){
		case 0:
			go=false;
			str="";
			break;
		case 8:
			if( curs ){
				str=str.substr( 0,curs-1 )+str.substr( curs );
				--curs;
			}
			break;
		case 27:
			curs=0;str="";
			break;
		case BBInputDriver::ASC_DELETE:
			if( curs<str.size() ) str=str.substr( 0,curs )+str.substr( curs+1 );
			break;
		case BBInputDriver::ASC_HOME:
			curs=0;
			break;
		case BBInputDriver::ASC_END:
			curs=str.size();
			break;
		case BBInputDriver::ASC_LEFT:
			if( curs ) --curs;
			break;
		case BBInputDriver::ASC_RIGHT:
			if( curs<str.size() ) ++curs;
			break;
		case '\r':case '\n':
			go=false;
			break;
		default:
			if( curr_font->isPrintable( key ) ){
				str=str.substr(0,curs)+char(key)+str.substr(curs);
				++curs;
			}
		}

		//render text
		p_canvas->blit( 0,curr_font->getHeight(),p_canvas,0,0,c->getWidth(),curr_font->getHeight(),true );
		p_canvas->text( curs_x,curr_font->getHeight(),str );
		c->blit( 0,curs_y,p_canvas,0,curr_font->getHeight(),c->getWidth(),curr_font->getHeight(),true );
	}

	curs_x=0;
	curs_y+=curr_font->getHeight();
	endPrinting( c );
	return d_new BBStr( str );
}

void BBCALL bbLocate( bb_int_t x,bb_int_t y ){
	BBCanvas *c=gx_graphics->getFrontCanvas();
	curs_x=x<0 ? 0 : (x > c->getWidth() ? c->getWidth() : x);
	curs_y=y<0 ? 0 : (y > c->getHeight() ? c->getHeight() : y);
}

void blitz2d_open(){
	curr_clsColor=0;
	curr_color=0xffffffff;
	curr_font=gx_graphics->getDefaultFont();
}

void blitz2d_reset(){
	curs_x=curs_y=0;
	gx_canvas->setOrigin( 0,0 );
	gx_canvas->setViewport( 0,0,gx_canvas->getWidth(),gx_canvas->getHeight() );
	gx_canvas->setColor( curr_color );
	gx_canvas->setClsColor( curr_clsColor );
	gx_canvas->setFont( curr_font );
	gx_canvas->set();
}

void blitz2d_close(){
	while( image_set.size() ) bbFreeImage( *image_set.begin() );
	if( p_canvas ){
		gx_graphics->freeCanvas( p_canvas );
		p_canvas=0;
	}
}

BBMODULE_CREATE( graphics ){
	if( FT_Init_FreeType(&ft) ) return false;

	filter=true;
	auto_dirty=true;
	auto_midhandle=false;
	p_canvas=0;
	gx_driver=0;
	gx_graphics=0;

	return true;
}

BBMODULE_DESTROY( graphics ){
	freeGraphics();
	gfx_modes.clear();
	return true;
}
