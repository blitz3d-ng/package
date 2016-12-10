#ifndef BBSCENE_H
#define BBSCENE_H

#include "../graphics/canvas.h"

class BBScene{
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
};

#endif
