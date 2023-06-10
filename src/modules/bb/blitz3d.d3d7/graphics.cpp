//////////////
// 3D STUFF //
//////////////

#include "graphics.h"

static int maxDevType;

static HRESULT CALLBACK enumDevice( char *desc,char *name,D3DDEVICEDESC7 *devDesc,void *context ){
	D3D7B3DGraphics *g=(D3D7B3DGraphics*)context;
	int t=0;
	GUID guid=devDesc->deviceGUID;
	if( guid==IID_IDirect3DRGBDevice ) t=1;
	else if( guid==IID_IDirect3DHALDevice ) t=2;
	else if( guid==IID_IDirect3DTnLHalDevice ) t=3;
	if( t>maxDevType ){
		g->dir3dDevDesc=*devDesc;
		maxDevType=t;
	}
	return D3DENUMRET_OK;
}

static HRESULT CALLBACK enumZbuffFormat( LPDDPIXELFORMAT format,void *context ){
	D3D7B3DGraphics *g=(D3D7B3DGraphics*)context;
	if( format->dwZBufferBitDepth==g->primFmt.dwRGBBitCount ){
		g->zbuffFmt=*format;
		return D3DENUMRET_CANCEL;
	}
	if( format->dwZBufferBitDepth>g->zbuffFmt.dwZBufferBitDepth ){
		if( format->dwZBufferBitDepth<g->primFmt.dwRGBBitCount ){
			g->zbuffFmt=*format;
		}
	}
	return D3DENUMRET_OK;
}

struct TexFmt{
	DDPIXELFORMAT fmt;
	int bits,a_bits,rgb_bits;
};

static int cntBits( int mask ){
	int n=0;
	for( int k=0;k<32;++k ){
		if( mask & (1<<k) ) ++n;
	}
	return n;
}

static std::vector<TexFmt> tex_fmts;

static HRESULT CALLBACK enumTextureFormat( DDPIXELFORMAT *fmt,void *p ){
	TexFmt t;
	t.fmt=*fmt;
	t.bits=fmt->dwRGBBitCount;
	t.a_bits=(fmt->dwFlags & DDPF_ALPHAPIXELS) ? cntBits(fmt->dwRGBAlphaBitMask) : 0;
	t.rgb_bits=(fmt->dwFlags & DDPF_RGB) ? cntBits(fmt->dwRBitMask|fmt->dwGBitMask|fmt->dwBBitMask) : 0;

	tex_fmts.push_back( t );

	return D3DENUMRET_OK;
}

static std::string itobin( int n ){
	std::string t;
	for( int k=0;k<32;n<<=1,++k ){
		t+=(n&0x80000000) ? '1' : '0';
	}
	return t;
}

static void debugPF( const DDPIXELFORMAT &pf ){
	std::string t;
	t="Bits:"+itoa( pf.dwRGBBitCount );
	_bbDebugLog( t.c_str() );
	t="R Mask:"+itobin( pf.dwRBitMask );
	_bbDebugLog( t.c_str() );
	t="G Mask:"+itobin( pf.dwGBitMask );
	_bbDebugLog( t.c_str() );
	t="B Mask:"+itobin( pf.dwBBitMask );
	_bbDebugLog( t.c_str() );
	t="A Mask:"+itobin( pf.dwRGBAlphaBitMask );
	_bbDebugLog( t.c_str() );
}

static void pickTexFmts( D3D7B3DGraphics *g,int hi ){
	//texRGBFmt.
	{
		int pick=-1,max=0,bits=0;
		for( int d=g->primFmt.dwRGBBitCount;d<=32;d+=8 ){
			for( int k=0;k<tex_fmts.size();++k ){
				const TexFmt &t=tex_fmts[k];
				if( t.bits>d || !t.rgb_bits || t.rgb_bits<max ) continue;
				if( t.rgb_bits==max && t.bits>=bits ) continue;
				pick=k;max=t.rgb_bits;bits=t.bits;
			}
			if( !hi && pick>=0 ) break;
		}
		if( pick<0 ) g->texRGBFmt[hi]=g->primFmt;
		else g->texRGBFmt[hi]=tex_fmts[pick].fmt;
	}
	//texAlphaFmt
	{
		int pick=-1,max=0,bits=0;
		for( int d=g->primFmt.dwRGBBitCount;d<=32;d+=8 ){
			for( int k=0;k<tex_fmts.size();++k ){
				const TexFmt &t=tex_fmts[k];
				if( t.bits>d || !t.a_bits || t.a_bits<max ) continue;
				if( t.a_bits==max && t.bits>=bits ) continue;
				pick=k;max=t.a_bits;bits=t.bits;
			}
			if( !hi && pick>=0 ) break;
		}
		if( pick<0 ) g->texAlphaFmt[hi]=g->primFmt;
		else g->texAlphaFmt[hi]=tex_fmts[pick].fmt;
	}
	//texRGBAlphaFmt
	{
		int pick=-1,a8rgb8=-1,max=0,bits=0;
		for( int d=g->primFmt.dwRGBBitCount;d<=32;d+=8 ){
			for( int k=0;k<tex_fmts.size();++k ){
				const TexFmt &t=tex_fmts[k];
				if( t.a_bits==8 && t.bits==16 ){ a8rgb8=k;continue; }
				if( t.bits>d || !t.a_bits || !t.rgb_bits || t.a_bits<max ) continue;
				if( t.a_bits==max && t.bits>=bits ) continue;
				pick=k;max=t.a_bits;bits=t.bits;
			}
			if( !hi && pick>=0 ) break;
		}
		if( pick<0 ) pick=a8rgb8;
		if( pick<0 ) g->texRGBAlphaFmt[hi]=g->primFmt;
		else g->texRGBAlphaFmt[hi]=tex_fmts[pick].fmt;
	}
	//texRGBMaskFmt...
	{
		int pick=-1,max=0,bits=0;
		for( int d=g->primFmt.dwRGBBitCount;d<=32;d+=8 ){
			for( int k=0;k<tex_fmts.size();++k ){
				const TexFmt &t=tex_fmts[k];
				if( !t.a_bits || !t.rgb_bits || t.rgb_bits<max ) continue;
				if( t.rgb_bits==max && t.bits>=bits ) continue;
				pick=k;max=t.rgb_bits;bits=t.bits;
			}
			if( !hi && pick>=0 ) break;
		}
		if( pick<0 ) g->texRGBMaskFmt[hi]=g->primFmt;
		else g->texRGBMaskFmt[hi]=tex_fmts[pick].fmt;
	}
}

BBScene *D3D7B3DGraphics::createScene( int w,int h,float d,int flags ){
	if( scene_set.size() ) return 0;

	//get d3d
	if( getDirDraw()->QueryInterface( IID_IDirect3D7,(void**)&dir3d )>=0 ){
		//enum devices
		maxDevType=0;
		if( dir3d->EnumDevices( enumDevice,this )>=0 && maxDevType>1 ){
			//enum zbuffer formats
			zbuffFmt.dwZBufferBitDepth=0;
			if( dir3d->EnumZBufferFormats( dir3dDevDesc.deviceGUID,enumZbuffFormat,this )>=0 ){
				//create zbuff for back buffer
				if( backCanvas()->attachZBuffer( zbuffFmt ) ){
					//create 3d device
					if( dir3d->CreateDevice( dir3dDevDesc.deviceGUID,backCanvas()->getSurface(),&dir3dDev )>=0 ){
						//enum texture formats
						tex_fmts.clear();
						if( dir3dDev->EnumTextureFormats( enumTextureFormat,this )>=0 ){
							pickTexFmts( this,0 );
							pickTexFmts( this,1 );
							tex_fmts.clear();
#ifdef BETA
							// _bbDebugLog( "Texture RGB format:" );
							// debugPF( texRGBFmt );
							// _bbDebugLog( "Texture Alpha format:" );
							// debugPF( texAlphaFmt );
							// _bbDebugLog( "Texture RGB Alpha format:" );
							// debugPF( texRGBAlphaFmt );
							// _bbDebugLog( "Texture RGB Mask format:" );
							// debugPF( texRGBMaskFmt );
							// _bbDebugLog( "Texture Primary format:" );
							// debugPF( primFmt );
							// string ts="ZBuffer Bit Depth:"+itoa( zbuffFmt.dwZBufferBitDepth );
							// _bbDebugLog( ts.c_str() );
#endif
							gxScene *scene=d_new gxScene( dir3d,dir3dDev,(gxCanvas*)backCanvas() );
							scene_set.insert( scene );

							dummy_mesh=(gxMesh*)scene->createMesh( 8,12,0 );
							return scene;
						}
						dir3dDev->Release();
						dir3dDev=0;
					}
					backCanvas()->releaseZBuffer();
				}
			}
		}
		dir3d->Release();
		dir3d=0;
	}
	return 0;
}

D3D7B3DGraphics::D3D7B3DGraphics():dir3d(0),dir3dDev(0),dummy_mesh(0){
}

D3D7B3DGraphics::~D3D7B3DGraphics(){
	dummy_mesh=0;

	if( dir3dDev ){ dir3dDev->Release();dir3dDev=0; }
	if( dir3d ){ dir3d->Release();dir3d=0; }
}
