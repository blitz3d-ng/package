
#include "../../stdutil/stdutil.h"
#include "graphics.d3d7.h"
#include <windows.h>
#include <d3d.h>
using namespace std;

static DDSURFACEDESC2 desktop_desc;

struct D3D7ContextDriver::GfxMode{
	DDSURFACEDESC2 desc;
};
struct D3D7ContextDriver::GfxDriver{
	GUID *guid;
	std::string name;
	std::vector<GfxMode*> modes;
#ifdef PRO
	D3DDEVICEDESC7 d3d_desc;
#endif
};

D3D7ContextDriver::D3D7ContextDriver():curr_driver(0),enum_all(false){
}

D3D7ContextDriver::~D3D7ContextDriver(){
}

bool D3D7ContextDriver::setDisplayMode( int w,int h,int d,bool d3d,IDirectDraw7 *dirDraw ){

	if( d ) return dirDraw->SetDisplayMode( w,h,d,0,0 )>=0;

	int best_d=0;

	if( d3d ){
#ifdef PRO
		int bd=curr_driver->d3d_desc.dwDeviceRenderBitDepth;
		if( bd & DDBD_32 ) best_d=32;
		else if( bd & DDBD_24 ) best_d=24;
		else if( bd & DDBD_16 ) best_d=16;
#endif
	}else{
		int best_n=0;
		for( d=16;d<=32;d+=8 ){
			if( dirDraw->SetDisplayMode( w,h,d,0,0 )<0 ) continue;
			DDCAPS caps={ sizeof(caps)  };
			dirDraw->GetCaps( &caps,0 );
			int n=0;
			if( caps.dwCaps & DDCAPS_BLT ) ++n;
			if( caps.dwCaps & DDCAPS_BLTCOLORFILL ) ++n;
			if( caps.dwCKeyCaps & DDCKEYCAPS_SRCBLT ) ++n;
			if( caps.dwCaps2 & DDCAPS2_WIDESURFACES ) ++n;
			if( n==4 ) return true;
			if( n>best_n ){
				best_d=d;
				best_n=n;
			}
			dirDraw->RestoreDisplayMode();
		}
	}
	return best_d ? dirDraw->SetDisplayMode( w,h,best_d,0,0 )>=0 : false;
}

////////////////////
// GFX ENUM STUFF //
////////////////////
static HRESULT WINAPI enumMode( DDSURFACEDESC2 *desc,void *context ){
	int dp=desc->ddpfPixelFormat.dwRGBBitCount;
	if( dp==16 || dp==24 || dp==32 ){
		D3D7ContextDriver::GfxMode *m=d_new D3D7ContextDriver::GfxMode;
		m->desc=*desc;
		D3D7ContextDriver::GfxDriver *d=(D3D7ContextDriver::GfxDriver*)context;
		d->modes.push_back( m );
	}
	return DDENUMRET_OK;
}

#ifdef PRO
static int maxDevType;
static HRESULT CALLBACK enumDevice( char *desc,char *name,D3DDEVICEDESC7 *devDesc,void *context ){
	int t=0;
	GUID guid=devDesc->deviceGUID;
	if( guid==IID_IDirect3DRGBDevice ) t=1;
	else if( guid==IID_IDirect3DHALDevice ) t=2;
	else if( guid==IID_IDirect3DTnLHalDevice ) t=3;
	if( t>1 && t>maxDevType ){
		maxDevType=t;
		D3D7ContextDriver::GfxDriver *d=(D3D7ContextDriver::GfxDriver*)context;
		d->d3d_desc=*devDesc;
	}
	return D3DENUMRET_OK;
}
#endif

static BOOL WINAPI enumDriver( GUID FAR *guid,LPSTR desc,LPSTR name,LPVOID context,HMONITOR hm ){
	IDirectDraw7 *dd;
	if( DirectDrawCreateEx( guid,(void**)&dd,IID_IDirectDraw7,0 )<0 ) return 0;

	if( !guid && !desktop_desc.ddpfPixelFormat.dwRGBBitCount ){
		desktop_desc.dwSize=sizeof(desktop_desc);
		dd->GetDisplayMode( &desktop_desc );
	}

	D3D7ContextDriver::GfxDriver *d=d_new D3D7ContextDriver::GfxDriver;

	d->guid=guid ? d_new GUID( *guid ) : 0;
	d->name=desc;//string( name )+" "+string( desc );

#ifdef PRO
	memset( &d->d3d_desc,0,sizeof(d->d3d_desc) );
	IDirect3D7 *dir3d;
	if( dd->QueryInterface( IID_IDirect3D7,(void**)&dir3d )>=0 ){
		maxDevType=0;
		dir3d->EnumDevices( enumDevice,d );
		dir3d->Release();
	}
#endif
	vector<D3D7ContextDriver::GfxDriver*> *drivers=(vector<D3D7ContextDriver::GfxDriver*>*)context;
	drivers->push_back( d );
	dd->EnumDisplayModes( 0,0,d,enumMode );
	dd->Release();
	return 1;
}

IDirectDraw7 *D3D7ContextDriver::createDD(){
	IDirectDraw7 *dd;
	if( DirectDrawCreateEx( curr_driver->guid,(void**)&dd,IID_IDirectDraw7,0 )<0 ) return 0;
	return dd;
}

void D3D7ContextDriver::enumGfx(){
	denumGfx();
	if( enum_all ){
		DirectDrawEnumerateEx( enumDriver,&drivers,DDENUM_ATTACHEDSECONDARYDEVICES|DDENUM_NONDISPLAYDEVICES );
	}else{
		DirectDrawEnumerateEx( enumDriver,&drivers,0 );
	}
}

void D3D7ContextDriver::denumGfx(){
	for( int k=0;k<drivers.size();++k ){
		D3D7ContextDriver::GfxDriver *d=drivers[k];
		for( int j=0;j<d->modes.size();++j ) delete d->modes[j];
		delete d->guid;
		delete d;
	}
	drivers.clear();
}

int D3D7ContextDriver::numGraphicsDrivers(){
	if( !enum_all ){
		enum_all=true;
		enumGfx();
	}
	return drivers.size();
}

void D3D7ContextDriver::graphicsDriverInfo( int driver,string *name,int *c ){
	GfxDriver *g=drivers[driver];
	int caps=0;
#ifdef PRO
	if( g->d3d_desc.dwDeviceRenderBitDepth ) caps|=GFXMODECAPS_3D;
#endif
	*name=g->name;
	*c=caps;
}

int D3D7ContextDriver::numGraphicsModes( int driver ){
	return drivers[driver]->modes.size();
}

void D3D7ContextDriver::graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
	GfxDriver *g=drivers[driver];
	GfxMode *m=g->modes[mode];
	int caps=0;
#ifdef PRO
	int bd=0;
	switch( m->desc.ddpfPixelFormat.dwRGBBitCount ){
	case 16:bd=DDBD_16;break;
	case 24:bd=DDBD_24;break;
	case 32:bd=DDBD_32;break;
	}
	if( g->d3d_desc.dwDeviceRenderBitDepth & bd && ( m->desc.dwWidth<=2048 && m->desc.dwHeight<=1536 ) ) caps|=GFXMODECAPS_3D;
#endif
	*w=m->desc.dwWidth;
	*h=m->desc.dwHeight;
	*d=m->desc.ddpfPixelFormat.dwRGBBitCount;
	*c=caps;
}

void D3D7ContextDriver::windowedModeInfo( int *c ){
	int caps=0;
#ifdef PRO
	int bd=0;
	switch( desktop_desc.ddpfPixelFormat.dwRGBBitCount ){
	case 16:bd=DDBD_16;break;
	case 24:bd=DDBD_24;break;
	case 32:bd=DDBD_32;break;
	}
	if( drivers[0]->d3d_desc.dwDeviceRenderBitDepth & bd ) caps|=GFXMODECAPS_3D;
#endif
	*c=caps;
}

BBMODULE_CREATE( graphics_d3d7 ){
	return true;
}

BBMODULE_DESTROY( graphics_d3d7 ){
	return true;
}
