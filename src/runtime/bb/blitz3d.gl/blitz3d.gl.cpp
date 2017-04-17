
#include "../../stdutil/stdutil.h"
#include <bb/blitz2d.gl/blitz2d.gl.h>
#include "blitz3d.gl.h"

#include <iostream>
using namespace std;

class GLLight : public BBLightRep{
  void update( Light *light ){
  }
};

class GLMesh : public BBMesh{
protected:
  bool dirty()const{ return false; }

public:
  float *v_coords,*v_normal,*v_tex_coord;
  unsigned int *v_color;
  int *tris;
  int max_verts,max_tris,flags;

  GLMesh( int mv,int mt,int f ):max_verts(mv),max_tris(mt),flags(f){
    v_coords=new float[max_verts*3];
    v_normal=new float[max_verts*3];
    v_tex_coord=new float[max_verts*2*2];
    v_color=new unsigned int[max_verts];
    tris=new int[max_tris*3];
  }

  bool lock( bool all ){
    cout<<"lock"<<endl;
  }

  void unlock(){
    cout<<"unlock"<<endl;
  }

  void setVertex( int n,const void *_v ){
    cout<<"setVertex1"<<endl;
    const Surface::Vertex *v=(const Surface::Vertex*)_v;
    v_coords[n*3+0]=v->coords.x;v_coords[n*3+1]=v->coords.y;v_coords[n*3+2]=v->coords.z;
    v_normal[n*3+0]=-v->normal.x;v_normal[n*3+1]=-v->normal.y;v_normal[n*3+2]=-v->normal.z;
    // memcpy( &v_tex_coord[n*2*2],v->tex_coords,2*2*sizeof(float) );
    v_tex_coord[n*2*2+0]=v->tex_coords[0][0];
    v_tex_coord[n*2*2+1]=v->tex_coords[0][1];
    v_tex_coord[n*2*2+2]=v->tex_coords[1][0];
    v_tex_coord[n*2*2+3]=v->tex_coords[1][1];
  }

  void setVertex( int n,const float coords[3],const float normal[3],const float tex_coords[2][2] ){
    cout<<"setVertex2"<<endl;
    v_coords[n*3+0]=coords[0];v_coords[n*3+1]=coords[1];v_coords[n*3+2]=coords[2];
    v_normal[n*3+0]=-normal[0];v_normal[n*3+1]=-normal[1];v_normal[n*3+2]=-normal[2];
    memcpy( &v_tex_coord,tex_coords,2*2*sizeof(float) );
  }

  void setVertex( int n,const float coords[3],const float normal[3],unsigned argb,const float tex_coords[2][2] ){
    cout<<"setVertex3"<<endl;
    v_coords[n*3+0]=coords[0];v_coords[n*3+1]=coords[1];v_coords[n*3+2]=coords[2];
    v_normal[n*3+0]=-normal[0];v_normal[n*3+1]=-normal[1];v_normal[n*3+2]=-normal[2];
    memcpy( &v_tex_coord,tex_coords,2*2*sizeof(float) );
    v_color[n]=argb;
  }

  void setTriangle( int n,int v0,int v1,int v2 ){
    cout<<"setTriangle"<<endl;
    tris[n*3+0]=v2;
    tris[n*3+1]=v1;
    tris[n*3+2]=v0;
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
    glScissor( x,y,w,h );
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
  }
  void setRenderState( const RenderState &state ){
    for( int i=0;i<MAX_TEXTURES;i++ ){
      glActiveTexture( GL_TEXTURE0+i );

      GLB2DTextureCanvas *canvas=(GLB2DTextureCanvas*)state.tex_states[i].canvas;

      if( !canvas ){
        cout<<"disabled: "<<i<<endl;
        glDisable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D,0 );
      } else {
        cout<<"enabled: "<<i<<" "<<canvas->getTextureId()<<endl;

        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D,canvas->getTextureId() );

        glMatrixMode( GL_TEXTURE );
        glLoadIdentity();

        if( true ){ // mipmap
          glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
        }else{
          glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
        }
      }
    }
  }

  //rendering
  bool begin( const std::vector<BBLightRep*> &lights ){
    cout<<"begin"<<endl;

    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_SCISSOR_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST );
    glEnable( GL_CULL_FACE );

    glEnable( GL_NORMALIZE );

    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE );

    glAlphaFunc( GL_GEQUAL,0.5 );

    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    float z=1.0f,w=0.0f;
    float pos[]={ 0.0,0.0,-z,w };
    float rgba[]={ 1.0f,1.0f,1.0f,1.0f };
    glLightfv( GL_LIGHT0,GL_POSITION,pos );
    glLightfv( GL_LIGHT0,GL_DIFFUSE,rgba );

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
      for( int i=0;i<mesh->max_tris;i++ ){
        for( int j=0;j<3;j++ ){
          int n=mesh->tris[i*3+j];

          float *v_coords=&mesh->v_coords[n*3],
                *v_normal=&mesh->v_normal[n*3],
                *v_tex_coord=&mesh->v_tex_coord[n*2*2];

          unsigned int v_color=mesh->v_color[n];

          int a = (v_color >> 24) & 255;
          int r = (v_color >> 16) & 255;
          int g = (v_color >> 8) & 255;
          int b = v_color & 255;

          glColor4ub( r,g,b,a );
          glVertex3fv( v_coords );
          glNormal3fv( v_normal );
          glTexCoord2f( 1.0f-v_tex_coord[0],1.0f-v_tex_coord[1] );
          cout<<v_tex_coord[0]<<", "<<v_tex_coord[1]<<endl;
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
    BBMesh *mesh=d_new GLMesh( max_verts,max_tris,flags );
    mesh_set.insert( mesh );
    return mesh;
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
