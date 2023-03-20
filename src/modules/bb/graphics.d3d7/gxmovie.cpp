#include "gxmovie.h"
#include <bb/runtime.windows/gxgraphics.h>

gxMovie::gxMovie( gxGraphics *g,IMultiMediaStream *mm )
:gfx(g),mm_stream( mm ),playing(true){

	mm_stream->GetMediaStream( MSPID_PrimaryVideo,&vid_stream );
	vid_stream->QueryInterface( IID_IDirectDrawMediaStream,(void**)&dd_stream );

	DDSURFACEDESC desc={sizeof(desc)};
	dd_stream->GetFormat( &desc,0,0,0 );

	canvas=(D3D7Canvas*)gfx->createCanvas( desc.dwWidth,desc.dwHeight,0 );	//D3D7Canvas::CANVAS_NONDISPLAY );
	canvas->getSurface()->QueryInterface( IID_IDirectDrawSurface,(void**)&dd_surf );

	src_rect.left=src_rect.top=0;
	src_rect.right=desc.dwWidth;src_rect.bottom=desc.dwHeight;

	dd_stream->CreateSample( dd_surf,&src_rect,0,&dd_sample );

	mm_stream->SetState( STREAMSTATE_RUN );
}

gxMovie::~gxMovie(){
	mm_stream->SetState( STREAMSTATE_STOP );

	dd_sample->Release();
	dd_surf->Release();
	dd_stream->Release();
	vid_stream->Release();
	mm_stream->Release();

	gfx->freeCanvas( canvas );
}

bool gxMovie::draw( BBCanvas *d,int x,int y,int w,int h ){
	D3D7Canvas *dest=(D3D7Canvas*)d;
	if( !playing ) return false;
	if( !dd_sample->Update( 0,0,0,0 ) ){
		RECT dest_rect={x,y,x+w,y+h};
		dest->getSurface()->Blt( &dest_rect,canvas->getSurface(),&src_rect,DDBLT_WAIT,0 );
		dest->damage( dest_rect );
	}else{
		playing=false;
	}
	return playing;
}
