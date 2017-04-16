
#include "../../stdutil/stdutil.h"
#include "blitz3d.gl.h"

#ifdef __MACH__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

#include <iostream>
using namespace std;

static GLfloat	rtri;				// Angle For The Triangle ( NEW )
static GLfloat	rquad;				// Angle For The Quad ( NEW )


class GLLight : public BBLightRep{
  void update( Light *light ){
  }
};

class GLMesh : public BBMesh{
protected:
  bool dirty()const{ return false; }

  int max_verts,max_tris,flags;
public:
  Surface::Vertex *verts;
  int *tris;

  GLMesh( int mv,int mt,int f ):max_verts(mv),max_tris(mt),flags(f){
    tris=new int[max_tris];
    verts=new Surface::Vertex[max_verts];
  }

  bool lock( bool all ){
    cout<<"lock"<<endl;
  }

  void unlock(){
    cout<<"unlock"<<endl;
  }

  void setVertex( int n,const void *_v ){
    const Surface::Vertex *v=(const Surface::Vertex*)_v;
    verts[n]=*v;
    cout<<"setVertex"<<endl;
  }

  void setVertex( int n,const float coords[3],const float normal[3],const float tex_coords[2][2] ){
    cout<<"setVertex"<<endl;
    verts[n].coords=coords;
    verts[n].normal=normal;
    memcpy( verts[n].tex_coords,tex_coords,sizeof(verts[n].tex_coords) );
  }

  void setVertex( int n,const float coords[3],const float normal[3],unsigned argb,const float tex_coords[2][2] ){
    cout<<"setVertex"<<endl;
    verts[n].coords=coords;
    verts[n].normal=normal;
    verts[n].color=argb;
    memcpy( verts[n].tex_coords,tex_coords,sizeof(verts[n].tex_coords) );
  }

  void setTriangle( int n,int v0,int v1,int v2 ){
    cout<<"setTriangle"<<endl;
    tris[n*3+0]=v0;
    tris[n*3+1]=v1;
    tris[n*3+2]=v2;
  }

};

class GLScene : public BBScene{
public:
  int  hwTexUnits(){ return 8; }
  int  gfxDriverCaps3D(){ return 0; }

  void setWBuffer( bool enable ){}
  void setHWMultiTex( bool enable ){}
  void setDither( bool enable ){}
  void setAntialias( bool enable ){}
  void setWireframe( bool enable ){}
  void setFlippedTris( bool enable ){}
  void setAmbient( const float rgb[3] ){
    float ambient[4]={ rgb[0],rgb[1],rgb[2],1.0f };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT,ambient );
  }
  void setAmbient2( const float rgb[3] ){
    setAmbient( rgb );
  }
  void setFogColor( const float rgb[3] ){}
  void setFogRange( float nr,float fr ){}
  void setFogMode( int mode ){}
  void setZMode( int mode ){}
  void setViewport( int x,int y,int w,int h ){
    glViewport( x,y,w,h );
    cout<<x<<", "<<y<<", "<<w<<", "<<h<<endl;
  }
  void setOrthoProj( float nr,float fr,float nr_w,float nr_h ){}
  void setPerspProj( float nr,float fr,float nr_w,float nr_h ){
    cout<<"proj"<<endl;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,640.0f/480.0f,0.1f,100.0f);
  }
  void setViewMatrix( const Matrix *matrix ){
    cout<<"set view matrix"<<endl;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef( 1.0f,1.0f,-1.0f );

    const Matrix *m=matrix;

    float mat[16]={
      m->elements[0][0], m->elements[0][1], m->elements[0][2], 0.0f,
      m->elements[1][0], m->elements[1][1], m->elements[1][2], 0.0f,
      m->elements[2][0], m->elements[2][1], m->elements[2][2], 0.0f,
      m->elements[3][0], m->elements[3][1], m->elements[3][2], 1.0f
    };

    glMultMatrixf( mat );
  }

  void setWorldMatrix( const Matrix *matrix ){
    cout<<"set world matrix"<<endl;
    glMatrixMode(GL_MODELVIEW);

    const Matrix *m=matrix;

    float mat[16]={
      m->elements[0][0], m->elements[0][1], m->elements[0][2], 0.0f,
      m->elements[1][0], m->elements[1][1], m->elements[1][2], 0.0f,
      m->elements[2][0], m->elements[2][1], m->elements[2][2], 0.0f,
      m->elements[3][0], m->elements[3][1], m->elements[3][2], 1.0f
    };

    glMultMatrixf( mat );

    // glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    // glLoadIdentity();									// Reset The Modelview Matrix

  }
  void setRenderState( const RenderState &state ){}

  //rendering
  bool begin( const std::vector<BBLightRep*> &lights ){
    cout<<"begin"<<endl;
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);

    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHT0);

    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE );
    glFrontFace( GL_CW );

    glAlphaFunc( GL_GEQUAL,0.5 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float z=1.0f,w=0.0f;
    float pos[]={ 0.0,0.0,-z,w };
    float rgba[]={ 1.0f,1.0f,1.0f,1.0f };
    glLightfv( GL_LIGHT0,GL_POSITION,pos );
    glLightfv( GL_LIGHT0,GL_DIFFUSE,rgba );

    rtri+=0.2f;
  	rquad-=0.15f;

    return true;
  }
  void clear( const float rgb[3],float alpha,float z,bool clear_argb,bool clear_z ){
    glClearColor( rgb[0],rgb[1],rgb[2],alpha );

    int clear_options=0;
    if( clear_argb ) clear_options|=GL_COLOR_BUFFER_BIT;
    if( clear_z ) clear_options|=GL_DEPTH_BUFFER_BIT;
    glClear( clear_options );
  }
  void render( BBMesh *m,int first_vert,int vert_cnt,int first_tri,int tri_cnt ){
    cout<<"render2"<<endl;

    GLMesh *mesh=(GLMesh*)m;

    glBegin(GL_TRIANGLES);
      for( int i=first_tri;i<tri_cnt;i++ ){
        for( int j=0;j<3;j++ ){
          Surface::Vertex *v=&mesh->verts[mesh->tris[i*3+j]];

          int a = (v->color >> 24) & 255;
          int r = (v->color >> 16) & 255;
          int g = (v->color >> 8) & 255;
          int b = v->color & 255;

          glColor4ub( r,g,b,a );
          glVertex3fv( v->coords );
          glNormal3fv( v->normal );
        }
      }
    glEnd();
  }
  void end(){
    cout<<"end"<<endl;
  }

  //lighting
  BBLightRep *createLight( int flags ){
    cout<<"create light"<<endl;
    return d_new GLLight();
  }
  void freeLight( BBLightRep *l ){}

  //meshes
  BBMesh *createMesh( int max_verts,int max_tris,int flags ){
    cout<<"create mesh"<<endl;
    return d_new GLMesh( max_verts,max_tris,flags );
  }

  //info
  int getTrianglesDrawn()const{ return 0; }
};

BBScene *GLB3DGraphics::createScene( int flags ){
  return d_new GLScene();
}

BBScene *GLB3DGraphics::verifyScene( BBScene *scene ){
  return scene;
}

void GLB3DGraphics::freeScene( BBScene *scene ){
}

BBMODULE_CREATE( blitz3d_gl ){
  return true;
}

BBMODULE_DESTROY( blitz3d_gl ){
  return true;
}
