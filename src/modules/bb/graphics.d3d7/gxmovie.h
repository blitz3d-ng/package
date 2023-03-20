
#ifndef GXMOVIE_H
#define GXMOVIE_H

#include "canvas.h"
#include <bb/graphics/movie.h>

#include <mmstream.h>    // multimedia stream interfaces
#include <amstream.h>    // DirectShow multimedia stream interfaces
#include <ddstream.h>    // DirectDraw multimedia stream interfaces

class gxGraphics;

class gxMovie : public BBMovie{

public:
	gxMovie( gxGraphics *gfx,IMultiMediaStream *mm_stream );
	~gxMovie();

private:
	bool playing;
	RECT src_rect;
	gxGraphics *gfx;
	D3D7Canvas *canvas;
	IDirectDrawSurface *dd_surf;
	IMediaStream *vid_stream;
	IDirectDrawMediaStream *dd_stream;
	IDirectDrawStreamSample *dd_sample;
	IMultiMediaStream *mm_stream;

	/***** GX INTERFACE *****/
public:

	bool draw( BBCanvas *dest,int x,int y,int w,int h );

	bool isPlaying()const{ return playing; }
	int getWidth()const{ return src_rect.right; }
	int getHeight()const{ return src_rect.bottom; }
};

#endif
