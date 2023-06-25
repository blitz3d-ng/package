
#include "gxgraphics.h"
#include <bb/system.windows/system.windows.h>
#include "gxmovie.h"

// ugly hack to avoid conflicts with GLFW
#include <initguid.h>
DEFINE_GUID(IID_IDirect3D7,0xf5049e77,0x4861,0x11d2,0xa4,0x7,0x0,0xa0,0xc9,0x6,0x29,0xa8);
DEFINE_GUID(IID_IDirect3DRGBDevice,0xa4665c60,0x2673,0x11cf,0xa3,0x1a,0x00,0xaa,0x00,0xb9,0x33,0x56);
DEFINE_GUID(IID_IDirect3DHALDevice,0x84e63de0,0x46aa,0x11cf,0x81,0x6f,0x00,0x00,0xc0,0x20,0x15,0x6e);
DEFINE_GUID(IID_IDirectDraw7,0x15e65ec0,0x3b9c,0x11d2,0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b);
DEFINE_GUID(IID_IDirect3DTnLHalDevice,0xf5049e78,0x4861,0x11d2,0xa4,0x7,0x0,0xa0,0xc9,0x6,0x29,0xa8);
DEFINE_GUID(IID_IDirectDrawGammaControl,0x69c11c3e,0xb46b,0x11d1,0xad,0x7a,0x00,0xc0,0x4f,0xc2,0x9b,0x4e);

// for some reason the mingw headers are missing this...
#ifndef DDSGR_CALIBRATE
#define DDSGR_CALIBRATE 0x00000001L
#endif

#define D3D7CANVAS(c) ((D3D7Canvas*)c)

gxGraphics::gxGraphics( IDirectDraw7 *dd,IDirectDrawSurface7 *fs,IDirectDrawSurface7 *bs,bool d3d ):
dirDraw(dd),def_font(0),gfx_lost(false){
	dirDraw->QueryInterface( IID_IDirectDraw,(void**)&ds_dirDraw );

	bbSceneDriver=this;

	front_canvas=d_new gxCanvas( dirDraw,fs,def_font,0 );
	back_canvas=d_new gxCanvas( dirDraw,bs,def_font,0 );

	front_canvas->cls();
	back_canvas->cls();

	def_font=loadFont( "courier",12,0 );

	front_canvas->setFont( def_font );
	back_canvas->setFont( def_font );

	memset(&primFmt,0,sizeof(primFmt));
	primFmt.dwSize=sizeof(primFmt);
	fs->GetPixelFormat( &primFmt );

	//are we fullscreen?
	_gamma=0;
	if( fs!=bs ){
		if( fs->QueryInterface( IID_IDirectDrawGammaControl,(void**)&_gamma )>=0 ){
			if( _gamma->GetGammaRamp( 0,&_gammaRamp )<0 ) _gamma=0;
		}
	}
	if( !_gamma ){
		for( int k=0;k<256;++k ) _gammaRamp.red[k]=_gammaRamp.blue[k]=_gammaRamp.green[k]=k;
	}
}

gxGraphics::~gxGraphics(){
	if( _gamma ) _gamma->Release();
	while( canvas_set.size() ) freeCanvas( *canvas_set.begin() );

	std::set<std::string>::iterator it;
	for( it=font_res.begin();it!=font_res.end();++it ) RemoveFontResource( (*it).c_str() );
	font_res.clear();

	D3D7CANVAS(back_canvas)->releaseZBuffer();

	delete back_canvas;
	delete front_canvas;

	ds_dirDraw->Release();

	dirDraw->RestoreDisplayMode();
	dirDraw->Release();
}

bool gxGraphics::init(){
	return true;
}

void gxGraphics::wipeSystemProperties(){
	bbSystemProperties.erase( "direct3d7" );
	bbSystemProperties.erase( "direct3ddevice7" );
	bbSystemProperties.erase( "directdraw7" );
}

void gxGraphics::setSystemProperties(){
	bbSystemProperties["direct3d7"]=itoa( (bb_int_t)dir3d );
	bbSystemProperties["direct3ddevice7"]=itoa( (bb_int_t)dir3dDev );
	bbSystemProperties["directdraw7"]=itoa( (bb_int_t)dirDraw );
}

void gxGraphics::setGamma( int r,int g,int b,float dr,float dg,float db ){
	_gammaRamp.red[r&255]=dr*257.0f;
	_gammaRamp.green[g&255]=dg*257.0f;
	_gammaRamp.blue[b&255]=db*257.0f;
}

void gxGraphics::updateGamma( bool calibrate ){
	if( !_gamma ) return;
	_gamma->SetGammaRamp( calibrate ? DDSGR_CALIBRATE : 0,&_gammaRamp );
}

void gxGraphics::getGamma( int r,int g,int b,float *dr,float *dg,float *db ){
	*dr=_gammaRamp.red[r&255]/257.0f;
	*dg=_gammaRamp.green[g&255]/257.0f;
	*db=_gammaRamp.blue[b&255]/257.0f;
}

void gxGraphics::backup(){
}

bool gxGraphics::restore(){

	while( dirDraw->TestCooperativeLevel()!=DD_OK ){

		if( dirDraw->TestCooperativeLevel()==DDERR_WRONGMODE ) return false;

		Sleep( 100 );
	}

	if( D3D7CANVAS(back_canvas)->getSurface()->IsLost()==DD_OK ) return true;

	dirDraw->RestoreAllSurfaces();

	//restore all canvases
	std::set<BBCanvas*>::iterator it;
	for( it=canvas_set.begin();it!=canvas_set.end();++it ){
		D3D7CANVAS(*it)->restore();
	}

#ifdef PRO
	std::set<BBScene*>::iterator scene_it;
	for( scene_it=scene_set.begin();scene_it!=scene_set.end();++scene_it ){
		((gxScene*)(*scene_it))->restore();
	}
	if( dir3d ) dir3d->EvictManagedTextures();
#endif

	return true;
}

BBFont *gxGraphics::getDefaultFont()const{
	return def_font;
}

void gxGraphics::vwait(){
	dirDraw->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN,0 );
}

void gxGraphics::copy( BBCanvas *d,int dx,int dy,int dw,int dh,BBCanvas *s,int sx,int sy,int sw,int sh ){
	gxCanvas *dest=(gxCanvas*)d,*src=(gxCanvas*)s;
	RECT r={ dx,dy,dx+dw,dy+dh };
	ddUtil::copy( dest->getSurface(),dx,dy,dw,dh,src->getSurface(),sx,sy,sw,sh );
	dest->damage( r );
}

int gxGraphics::getScanLine()const{
	DWORD t=0;
	dirDraw->GetScanLine( &t );
	return t;
}

int gxGraphics::getTotalVidmem()const{
	DDCAPS caps={sizeof(caps)};
	dirDraw->GetCaps( &caps,0 );
	return caps.dwVidMemTotal;
}

int gxGraphics::getAvailVidmem()const{
	DDCAPS caps={sizeof(caps)};
	dirDraw->GetCaps( &caps,0 );
	return caps.dwVidMemFree;
}

BBMovie *gxGraphics::openMovie( const std::string &file,int flags ){

	IAMMultiMediaStream *iam_stream;

	if( CoCreateInstance(
		CLSID_AMMultiMediaStream,NULL,CLSCTX_INPROC_SERVER,
        IID_IAMMultiMediaStream,(void **)&iam_stream )==S_OK ){

		if( iam_stream->Initialize( STREAMTYPE_READ,AMMSF_NOGRAPHTHREAD,NULL )==S_OK ){

			if( iam_stream->AddMediaStream( ds_dirDraw,&MSPID_PrimaryVideo,0,NULL )==S_OK ){

				iam_stream->AddMediaStream( NULL,&MSPID_PrimaryAudio,AMMSF_ADDDEFAULTRENDERER,NULL );

				WCHAR *path=new WCHAR[ file.size()+1 ];
				MultiByteToWideChar( CP_ACP,0,file.c_str(),-1,path,sizeof(WCHAR)*(file.size()+1) );
				int n=iam_stream->OpenFile( path,0 );
				delete path;

				if( n==S_OK ){
					gxMovie *movie=d_new gxMovie( this,iam_stream );
					movie_set.insert( movie );
					return movie;
				}
			}
		}
		iam_stream->Release();
	}
	return 0;
}

BBCanvas *gxGraphics::createCanvas( int w,int h,int flags ){
	ddSurf *s=ddUtil::createSurface( w,h,flags,this );
	if( !s ) return 0;
	gxCanvas *c=d_new gxCanvas( dirDraw,s,def_font,flags );
	canvas_set.insert( c );
	c->cls();
	return c;
}

BBCanvas *gxGraphics::loadCanvas( const std::string &f,int flags ){
	ddSurf *s=ddUtil::loadSurface( f,flags,this );
	if( !s ) return 0;
	gxCanvas *c=d_new gxCanvas( dirDraw,s,def_font,flags );
	canvas_set.insert( c );
	return c;
}

int gxGraphics::getWidth()const{
	return front_canvas->getWidth();
}

int gxGraphics::getHeight()const{
	return front_canvas->getHeight();
}

int gxGraphics::getLogicalWidth()const{
	return front_canvas->getWidth();
}

int gxGraphics::getLogicalHeight()const{
	return front_canvas->getHeight();
}

int gxGraphics::getDepth()const{
	return front_canvas->getDepth();
}

float gxGraphics::getDensity()const{
	return 1.0;
}

BBFont *gxGraphics::loadFont( const std::string &f,int height,int flags ){
	int bold=flags & BBFont::FONT_BOLD ? FW_BOLD : FW_REGULAR;
	int italic=flags & BBFont::FONT_ITALIC ? 1 : 0;
	int underline=flags & BBFont::FONT_UNDERLINE ? 1 : 0;
	int strikeout=0;

	std::string t;
	int n=f.find('.');
	if( n!=std::string::npos ){
		t=fullfilename(f);
		if( !font_res.count(t) && AddFontResource( t.c_str() ) ) font_res.insert( t );
		t=filenamefile( f.substr(0,n) );
	}else{
		t=f;
	}

	//save and turn off font smoothing....
	BOOL smoothing=FALSE;
	SystemParametersInfo( SPI_GETFONTSMOOTHING,0,&smoothing,0 );
	SystemParametersInfo( SPI_SETFONTSMOOTHING,FALSE,0,0 );

	HFONT hfont=CreateFont(
		height,0,0,0,
		bold,italic,underline,strikeout,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,t.c_str() );

	if( !hfont ){
		//restore font smoothing
		SystemParametersInfo( SPI_SETFONTSMOOTHING,smoothing,0,0 );
		return 0;
	}
	HDC hdc=CreateCompatibleDC( 0 );
	HFONT pfont=(HFONT)SelectObject( hdc,hfont );

	TEXTMETRIC tm={0};
	if( !GetTextMetrics( hdc,&tm ) ){
		SelectObject( hdc,pfont );
		DeleteDC( hdc );
		DeleteObject( hfont );
		SystemParametersInfo( SPI_SETFONTSMOOTHING,smoothing,0,0 );
		return 0;
	}
	height=tm.tmHeight;

	int first=tm.tmFirstChar,last=tm.tmLastChar;
	int sz=last-first+1;
	int *offs=d_new int[sz];
	int *widths=d_new int[sz];
	int *as=d_new int[sz];

	//calc size of canvas to hold font.
	int x=0,y=0,max_x=0;
	for( int k=0;k<sz;++k ){

		char t=k+first;

		SIZE sz;
		GetTextExtentPoint32( hdc,&t,1,&sz );
		int w=sz.cx;

		as[k]=0;

		ABC abc;
		if( GetCharABCWidths( hdc,t,t,&abc ) ){
			if( abc.abcA<0 ){
				as[k]=ceil(-abc.abcA);
				w+=as[k];
			}
			if( abc.abcC<0 ) w+=ceil(-abc.abcC);
		}

		if( x && x+w>getWidth() ){ x=0;y+=height; }
		offs[k]=(x<<16)|y;
		widths[k]=w;
		x+=w;if( x>max_x ) max_x=x;
	}
	SelectObject( hdc,pfont );
	DeleteDC( hdc );

	int cw=max_x,ch=y+height;
	if( gxCanvas *c=(gxCanvas*)createCanvas( cw,ch,0 ) ){
		ddSurf *surf=c->getSurface();

		HDC surf_hdc;
		if( surf->GetDC( &surf_hdc )>=0 ){
			HFONT pfont=(HFONT)SelectObject( surf_hdc,hfont );

			SetBkColor( surf_hdc,0x000000 );
			SetTextColor( surf_hdc,0xffffff );

			for( int k=0;k<sz;++k ){
				int x=(offs[k]>>16)&0xffff,y=offs[k]&0xffff;
				char t=k+first;
				RECT rect={x,y,x+widths[k],y+height};
				ExtTextOut( surf_hdc,x+as[k],y,ETO_CLIPPED,&rect,&t,1,0 );
			}

			SelectObject( surf_hdc,pfont );
			surf->ReleaseDC( surf_hdc );
			DeleteObject( hfont );
			delete[] as;

			c->backup();
			D3D7Font *font=d_new D3D7Font( this,c,tm.tmMaxCharWidth,height,first,last+1,tm.tmDefaultChar,offs,widths );
			font_set.insert( font );

			//restore font smoothing
			SystemParametersInfo( SPI_SETFONTSMOOTHING,smoothing,0,0 );
			return font;
		}else{
		}
		freeCanvas( c );
	}else{
	}
	DeleteObject( hfont );
	delete[] as;
	delete[] widths;
	delete[] offs;

	//restore font smoothing
	SystemParametersInfo( SPI_SETFONTSMOOTHING,smoothing,0,0 );
	return 0;
}
