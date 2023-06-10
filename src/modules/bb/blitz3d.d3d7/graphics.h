#ifndef BB_BLITZ3D_D3D7_GRAPHICS_H
#define BB_BLITZ3D_D3D7_GRAPHICS_H

#include <bb/blitz3d/graphics.h>
#include "blitz3d.d3d7.h"
#include <d3d.h>

class D3D7B3DGraphics : public B3DGraphics{
public:
	D3D7B3DGraphics();
	~D3D7B3DGraphics();

	IDirect3D7 *dir3d;
	IDirect3DDevice7 *dir3dDev;
	D3DDEVICEDESC7 dir3dDevDesc;
	DDPIXELFORMAT primFmt,zbuffFmt;

	DDPIXELFORMAT texRGBFmt[2],texAlphaFmt[2],texRGBAlphaFmt[2],texRGBMaskFmt[2];

	virtual	IDirectDraw7 *getDirDraw()=0;
	virtual	D3D7Canvas *backCanvas()=0;

	BBScene *createScene( int w,int h,float d,int flags );

private:
	gxMesh *dummy_mesh;
};


#endif
