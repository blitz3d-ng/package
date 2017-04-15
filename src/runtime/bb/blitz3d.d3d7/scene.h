
#ifndef GXSCENE_H
#define GXSCENE_H

#include <map>
#include <d3d.h>

#include <bb/graphics.d3d7/canvas.h>

#include <bb/blitz3d/scene.h>
#include <bb/blitz3d/mesh.h>
#include "light.h"

class gxScene : public BBScene{
public:
	IDirect3D7 *dir3d;
	IDirect3DDevice7 *dir3dDev;

	gxScene( IDirect3D7 *dir3d,IDirect3DDevice7 *d,gxCanvas *target );
	~gxScene();

	void setEnabled( BBLightRep *light,bool enabled );

	void restore();

	/***** GX INTERFACE *****/
public:
	//state
	int  hwTexUnits();
	int  gfxDriverCaps3D();

	void setWBuffer( bool enable );
	void setHWMultiTex( bool enable );
	void setDither( bool enable );
	void setAntialias( bool enable );
	void setWireframe( bool enable );
	void setFlippedTris( bool enable );
	void setAmbient( const float rgb[3] );
	void setAmbient2( const float rgb[3] );
	void setFogColor( const float rgb[3] );
	void setFogRange( float nr,float fr );
	void setFogMode( int mode );
	void setZMode( int mode );
	void setViewport( int x,int y,int w,int h );
	void setOrthoProj( float nr,float fr,float nr_w,float nr_h );
	void setPerspProj( float nr,float fr,float nr_w,float nr_h );
	void setViewMatrix( const Matrix *matrix );
	void setWorldMatrix( const Matrix *matrix );
	void setRenderState( const RenderState &state );

	//rendering
	bool begin( const std::vector<BBLightRep*> &lights );
	void clear( const float rgb[3],float alpha,float z,bool clear_argb,bool clear_z );
	void render( BBMesh *mesh,int first_vert,int vert_cnt,int first_tri,int tri_cnt );
	void end();

	//lighting
	BBLightRep *createLight( int flags );
	void freeLight( BBLightRep *l );

	//meshes
	BBMesh *createMesh( int max_verts,int max_tris,int flags );

	//info
	int getTrianglesDrawn()const;

private:
	gxCanvas *target;
	bool wbuffer,dither,antialias,wireframe,flipped;
	unsigned ambient,ambient2,fogcolor;
	int caps_level,fogmode,zmode;
	float fogrange_nr,fogrange_fr;
	D3DVIEWPORT7 viewport;
	bool ortho_proj;
	float frustum_nr,frustum_fr,frustum_w,frustum_h;
	D3DMATRIX projmatrix,viewmatrix,worldmatrix;
	D3DMATRIX inv_viewmatrix;
	D3DMATERIAL7 material;
	float shininess;
	int blend,fx;
	struct TexState{
		gxCanvas *canvas;
		int blend,flags;
		D3DMATRIX matrix;
		bool mat_valid;
	};
	TexState texstate[MAX_TEXTURES];
	int n_texs,tris_drawn;

	std::set<BBLightRep*> _allLights;
	std::vector<BBLightRep*> _curLights;

	int d3d_rs[160];
	int d3d_tss[8][32];

	void setRS( int n,int t );
	void setTSS( int n,int s,int t );

	void setLights();
	void setZMode();
	void setAmbient();
	void setFogMode();
	void setTriCull();
	void setTexState( int index,const TexState &state,bool set_blend );
};

#endif
