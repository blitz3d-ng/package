
#include "canvas.h"

D3D7Canvas::D3D7Canvas( IDirectDraw7 *dd,IDirectDrawSurface7 *s,int f ):
dirDraw(dd),main_surf(s),surf(0),z_surf(0),t_surf(0),
locked_cnt(0),mod_cnt(0),remip_cnt(0),
cube_mode(CUBEMODE_REFLECTION|CUBESPACE_WORLD){
	flags=f;

  if( flags & CANVAS_TEX_CUBE ){
    cube_surfs[2]=main_surf;
    for( int k=0;k<6;++k ){
      if( k==2 ) continue;
      DWORD n;
      switch( k ){
      case 0:n=DDSCAPS2_CUBEMAP_NEGATIVEX;break;
      case 1:n=DDSCAPS2_CUBEMAP_POSITIVEZ;break;
      case 2:n=DDSCAPS2_CUBEMAP_POSITIVEX;break;
      case 3:n=DDSCAPS2_CUBEMAP_NEGATIVEZ;break;
      case 4:n=DDSCAPS2_CUBEMAP_POSITIVEY;break;
      case 5:n=DDSCAPS2_CUBEMAP_NEGATIVEY;break;
      default:return;
      }
      DDSCAPS2 caps={0};
      caps.dwCaps2=DDSCAPS2_CUBEMAP|n;
      main_surf->GetAttachedSurface( &caps,&cube_surfs[k] );
    }
    surf=cube_surfs[1];
  }else{
    surf=main_surf;
    memset( cube_surfs,0,sizeof(cube_surfs) );
  }

  DDSURFACEDESC2 desc={sizeof(desc)};
  surf->GetSurfaceDesc( &desc );
  format.setFormat( desc.ddpfPixelFormat );
}

D3D7Canvas::~D3D7Canvas(){
  if( locked_cnt ) surf->Unlock( 0 );
  if( t_surf ) t_surf->Release();
  releaseZBuffer();
  main_surf->Release();
}

void D3D7Canvas::backup()const{
	if( flags & CANVAS_TEX_CUBE ) return;

	if( !t_surf ){
		DDSURFACEDESC2 desc={sizeof(desc)};
		if( surf->GetSurfaceDesc(&desc)<0 ) return;
		if( desc.ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY ) return;

		DDSURFACEDESC2 t_desc={sizeof(t_desc)};
		t_desc.dwFlags=DDSD_CAPS|DDSD_PIXELFORMAT|DDSD_WIDTH|DDSD_HEIGHT;
		t_desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
		t_desc.dwWidth=desc.dwWidth;t_desc.dwHeight=desc.dwHeight;
		t_desc.ddpfPixelFormat=desc.ddpfPixelFormat;

		if( dirDraw->CreateSurface( &t_desc,&t_surf,0 )<0 ){
			t_surf=0;
			return;
		}
	}

	if( t_surf->Blt( 0,surf,0,DDBLT_WAIT,0 )<0 ) return;
}

void D3D7Canvas::restore()const{
	if( !t_surf ) return;

	if( surf->Blt( 0,t_surf,0,DDBLT_WAIT,0 )<0 ) return;
}

ddSurf *D3D7Canvas::getSurface()const{
	return surf;
}

ddSurf *D3D7Canvas::getTexSurface()const{
	if( mod_cnt==remip_cnt ) return main_surf;
	ddUtil::buildMipMaps( surf );
	remip_cnt=mod_cnt;
	return main_surf;
}

void D3D7Canvas::setModify( int n ){
	mod_cnt=n;
}

int D3D7Canvas::getModify()const{
	return mod_cnt;
}

bool D3D7Canvas::attachZBuffer( DDPIXELFORMAT zbuffFmt ){
	if( z_surf ) return true;
	DDSURFACEDESC2 desc={sizeof(desc)};
	desc.dwFlags=DDSD_WIDTH|DDSD_HEIGHT|DDSD_CAPS|DDSD_PIXELFORMAT;
	desc.dwWidth=getWidth();
	desc.dwHeight=getHeight();
	desc.ddsCaps.dwCaps=DDSCAPS_ZBUFFER|DDSCAPS_VIDEOMEMORY;
	desc.ddpfPixelFormat=zbuffFmt;
	if( dirDraw->CreateSurface( &desc,&z_surf,0 )<0 ) return false;
	surf->AddAttachedSurface( z_surf );
	return true;
}

void D3D7Canvas::releaseZBuffer(){
	if( !z_surf ) return;
	surf->DeleteAttachedSurface( 0,z_surf );
	z_surf->Release();
	z_surf=0;
}

bool D3D7Canvas::getZBufferFormat( DDPIXELFORMAT &fmt ){
	if( !z_surf ) return false;
	DDSURFACEDESC2 desc={sizeof(desc)};
	z_surf->GetSurfaceDesc( &desc );
	fmt=desc.ddpfPixelFormat;
	return true;
}

void D3D7Canvas::damage( const RECT &r )const{
	++mod_cnt;
}
