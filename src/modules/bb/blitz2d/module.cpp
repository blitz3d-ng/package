#include "../stdutil/stdutil.h"
#include "blitz2d.h"
#include <bb/blitz/module.h>

#include <bb/input/input.h>
#include <bb/system/system.h>
#include <bb/runtime/runtime.h>

#ifdef WIN32
#include <windows.h>
#endif

//degrees to radians
static const float dtor=0.0174532925199432957692369076848861f;

#include <fstream>
using namespace std;

#define b2d_graphics ((B2DGraphics*)gx_graphics)

class bbImage{
public:
	bbImage( const vector<BBCanvas*> &f ):frames(f){
	}
	~bbImage(){
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
static set<bbImage*> image_set;
/*static*/ int curs_x=0,curs_y=0;
static BBCanvas *p_canvas=0;
/*static*/ BBFont *curr_font=0;
/*static*/ unsigned curr_color=0;
/*static*/ unsigned curr_clsColor=0;

static inline void debugImage( bbImage *i,int frame=0 ){
	if( bb_env.debug ){
		if( !image_set.count(i) ) RTEX( "Image does not exist" );
		if( frame>=i->getFrames().size() ) RTEX( "Image frame out of range" );
	}
}

static inline void debugFont( BBFont *f ){
	if( bb_env.debug ){
		if( !b2d_graphics->verifyFont( f ) ) RTEX( "Font does not exist" );
	}
}

static inline void debugCanvas( BBCanvas *c ){
	if( bb_env.debug ){
		if( !gx_graphics->verifyCanvas( c ) ) RTEX( "Buffer does not exist" );
	}
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
	BBFont *font=b2d_graphics->loadFont( *name,height,flags );
	delete name;
	return font;
}

void BBCALL bbFreeFont( BBFont *f ){
	debugFont( f );
	if( f==curr_font ) bbSetFont( gx_graphics->getDefaultFont() );
	b2d_graphics->freeFont( f );
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

bbImage * BBCALL bbLoadImage( BBStr *s ){
	string t=*s;delete s;
	BBCanvas *c=gx_graphics->loadCanvas( t,0 );
	if( !c ) return 0;
	if( auto_dirty ) c->backup();
	if( auto_midhandle ) c->setHandle( c->getWidth()/2,c->getHeight()/2 );
	vector<BBCanvas*> frames;
	frames.push_back( c );
	bbImage *i=d_new bbImage( frames );
	image_set.insert( i );
	return i;
}

bbImage * BBCALL bbLoadAnimImage( BBStr *s,bb_int_t w,bb_int_t h,bb_int_t first,bb_int_t cnt ){

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
	bbImage *i=d_new bbImage( frames );
	image_set.insert( i );
	return i;
}

bbImage * BBCALL bbCopyImage( bbImage *i ){
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
	bbImage *t=d_new bbImage( frames );
	image_set.insert( t );
	return t;
}

bbImage * BBCALL bbCreateImage( bb_int_t w,bb_int_t h,bb_int_t n ){
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
	bbImage *i=d_new bbImage( frames );
	image_set.insert( i );
	return i;
}

void BBCALL bbFreeImage( bbImage *i ){
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

bb_int_t BBCALL bbSaveImage( bbImage *i,BBStr *str,bb_int_t n ){
	debugImage( i,n );
	string t=*str;delete str;
	BBCanvas *c=i->getFrames()[n];
	return saveCanvas( c,t ) ? 1 : 0;
}

void BBCALL bbGrabImage( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t n ){
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

BBCanvas * BBCALL bbImageBuffer( bbImage *i,bb_int_t n ){
	debugImage( i,n );
	return i->getFrames()[n];
}

void BBCALL bbDrawImage( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t frame ){
	debugImage( i,frame );
	BBCanvas *c=i->getFrames()[frame];
	gx_canvas->image( c,x,y,false );
}

void BBCALL bbDrawBlock( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t frame ){
	debugImage( i,frame );
	BBCanvas *c=i->getFrames()[frame];
	gx_canvas->image( c,x,y,true );
}

static void tile( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t frame,bool solid ){
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

void BBCALL bbTileImage( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t frame ){
	debugImage( i,frame );
	tile( i,x,y,frame,false );
}

void BBCALL bbTileBlock( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t frame ){
	debugImage( i,frame );
	tile( i,x,y,frame,true );
}

void BBCALL bbDrawImageRect( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t r_x,bb_int_t r_y,bb_int_t r_w,bb_int_t r_h,bb_int_t frame ){
	debugImage( i,frame );
	BBCanvas *c=i->getFrames()[frame];
	gx_canvas->blit( x,y,c,r_x,r_y,r_w,r_h,false );
}

void BBCALL bbDrawBlockRect( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t r_x,bb_int_t r_y,bb_int_t r_w,bb_int_t r_h,bb_int_t frame ){
	debugImage( i,frame );
	BBCanvas *c=i->getFrames()[frame];
	gx_canvas->blit( x,y,c,r_x,r_y,r_w,r_h,true );
}

void BBCALL bbMaskImage( bbImage *i,bb_int_t r,bb_int_t g,bb_int_t b ){
	debugImage( i );
	unsigned argb=(r<<16)|(g<<8)|b;
	const vector<BBCanvas*> &f=i->getFrames();
	for( int k=0;k<f.size();++k ) f[k]->setMask( argb );
}

void BBCALL bbHandleImage( bbImage *i,bb_int_t x,bb_int_t y ){
	debugImage( i );
	const vector<BBCanvas*> &f=i->getFrames();
	for( int k=0;k<f.size();++k ) f[k]->setHandle( x,y );
}

void BBCALL bbMidHandle( bbImage *i ){
	debugImage( i );
	const vector<BBCanvas*> &f=i->getFrames();
	for( int k=0;k<f.size();++k ) f[k]->setHandle( f[k]->getWidth()/2,f[k]->getHeight()/2 );
}

void BBCALL bbAutoMidHandle( bb_int_t enable ){
	auto_midhandle=enable ? true : false;
}

bb_int_t BBCALL bbImageWidth( bbImage *i ){
	debugImage( i );
	return i->getFrames()[0]->getWidth();
}

bb_int_t BBCALL bbImageHeight( bbImage *i ){
	debugImage( i );
	return i->getFrames()[0]->getHeight();
}

bb_int_t BBCALL bbImageXHandle( bbImage *i ){
	debugImage( i );
	int x,y;
	i->getFrames()[0]->getHandle( &x,&y );
	return x;
}

bb_int_t BBCALL bbImageYHandle( bbImage *i ){
	debugImage( i );
	int x,y;
	i->getFrames()[0]->getHandle( &x,&y );
	return y;
}

bb_int_t BBCALL bbImagesOverlap( bbImage *i1,bb_int_t x1,bb_int_t y1,bbImage *i2,bb_int_t x2,bb_int_t y2 ){
	debugImage( i1 );
	debugImage( i2 );
	BBCanvas *c1=i1->getFrames()[0];
	BBCanvas *c2=i2->getFrames()[0];
	return c1->collide( x1,y1,c2,x2,y2,true );
}

bb_int_t BBCALL bbImagesCollide( bbImage *i1,bb_int_t x1,bb_int_t y1,bb_int_t f1,bbImage *i2,bb_int_t x2,bb_int_t y2,bb_int_t f2 ){
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

bb_int_t BBCALL bbImageRectOverlap( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t x2,bb_int_t y2,bb_int_t w2,bb_int_t h2 ){
	debugImage( i );
	BBCanvas *c=i->getFrames()[0];
	return c->rect_collide( x,y,x2,y2,w2,h2,true );
}

bb_int_t BBCALL bbImageRectCollide( bbImage *i,bb_int_t x,bb_int_t y,bb_int_t f,bb_int_t x2,bb_int_t y2,bb_int_t w2,bb_int_t h2 ){
	debugImage( i,f );
	BBCanvas *c=i->getFrames()[f];
	return c->rect_collide( x,y,x2,y2,w2,h2,false );
}

void BBCALL bbTFormImage( bbImage *i,bb_float_t a,bb_float_t b,bb_float_t c,bb_float_t d ){
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

void BBCALL bbScaleImage( bbImage *i,bb_float_t w,bb_float_t h ){
	debugImage( i );
	bbTFormImage( i,w,0,0,h );
}

void BBCALL bbResizeImage( bbImage *i,bb_float_t w,bb_float_t h ){
	debugImage( i );
	BBCanvas *c=i->getFrames()[0];
	bbTFormImage( i,w/(float)c->getWidth(),0,0,h/(float)c->getHeight() );
}

void BBCALL bbRotateImage( bbImage *i,bb_float_t d ){
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

BBMODULE_CREATE( blitz2d ){
	if( FT_Init_FreeType(&ft) ) return false;

	filter=true;
	auto_dirty=true;
	auto_midhandle=false;
	p_canvas=0;

  return true;
}

BBMODULE_DESTROY( blitz2d ){
  return true;
}
