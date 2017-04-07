#ifndef BBSCENE_H
#define BBSCENE_H

#include "../graphics/canvas.h"
#include "light.h"
#include "mesh.h"

class BBScene{
  /***** GX INTERFACE *****/
public:
  enum{
    MAX_TEXTURES=	8
  };
  enum{
    FX_FULLBRIGHT=	0x0001,
    FX_VERTEXCOLOR=	0x0002,
    FX_FLATSHADED=	0x0004,
    FX_NOFOG=		0x0008,
    FX_DOUBLESIDED=	0x0010,
    FX_VERTEXALPHA=	0x0020,
    FX_WIREFRAME=	0x0040,

    FX_ALPHATEST=	0x2000,
    FX_CONDLIGHT=	0x4000,
    FX_EMISSIVE=	0x8000
  };
  enum{
    BLEND_REPLACE=	0,
    BLEND_ALPHA=	1,
    BLEND_MULTIPLY=	2,
    BLEND_ADD=		3,
    BLEND_DOT3=		4,
    BLEND_MULTIPLY2=5,
  };
  enum{
    ZMODE_NORMAL=	0,
    ZMODE_DISABLE=	1,
    ZMODE_CMPONLY=	2
  };
  enum{
    FOG_NONE=		0,
    FOG_LINEAR=		1
  };
  enum{
    TEX_COORDS2=	0x0001
  };
  struct Matrix{
    float elements[4][3];
  };
  struct RenderState{
    float color[3];
    float shininess,alpha;
    int blend,fx;
    struct TexState{
      BBCanvas *canvas;
      const Matrix *matrix;
      int blend,flags;
    }tex_states[MAX_TEXTURES];
  };

  //state
  virtual int  hwTexUnits()=0;
  virtual int  gfxDriverCaps3D()=0;

  virtual void setWBuffer( bool enable )=0;
  virtual void setHWMultiTex( bool enable )=0;
  virtual void setDither( bool enable )=0;
  virtual void setAntialias( bool enable )=0;
  virtual void setWireframe( bool enable )=0;
  virtual void setFlippedTris( bool enable )=0;
  virtual void setAmbient( const float rgb[3] )=0;
  virtual void setAmbient2( const float rgb[3] )=0;
  virtual void setFogColor( const float rgb[3] )=0;
  virtual void setFogRange( float nr,float fr )=0;
  virtual void setFogMode( int mode )=0;
  virtual void setZMode( int mode )=0;
  virtual void setViewport( int x,int y,int w,int h )=0;
  virtual void setOrthoProj( float nr,float fr,float nr_w,float nr_h )=0;
  virtual void setPerspProj( float nr,float fr,float nr_w,float nr_h )=0;
  virtual void setViewMatrix( const Matrix *matrix )=0;
  virtual void setWorldMatrix( const Matrix *matrix )=0;
  virtual void setRenderState( const RenderState &state )=0;

  //rendering
  virtual bool begin( const std::vector<BBLightRep*> &lights )=0;
  virtual void clear( const float rgb[3],float alpha,float z,bool clear_argb,bool clear_z )=0;
  virtual void render( BBMesh *mesh,int first_vert,int vert_cnt,int first_tri,int tri_cnt )=0;
  virtual void end()=0;

  //lighting
  virtual BBLightRep *createLight( int flags )=0;
  virtual void freeLight( BBLightRep *l )=0;

  //info
  virtual int getTrianglesDrawn()const=0;

};

#endif
