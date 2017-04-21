
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
		return true;
  }

  void unlock(){
  }

  void setVertex( int n,const void *_v ){
    const Surface::Vertex *v=(const Surface::Vertex*)_v;
    v_coords[n*3+0]=v->coords.x;v_coords[n*3+1]=v->coords.y;v_coords[n*3+2]=v->coords.z;
    v_normal[n*3+0]=-v->normal.x;v_normal[n*3+1]=-v->normal.y;v_normal[n*3+2]=-v->normal.z;
    // memcpy( &v_tex_coord[n*2*2],v->tex_coords,2*2*sizeof(float) );
		v_color[n]=0xffffff;
    v_tex_coord[n*2*2+0]=v->tex_coords[0][0];
    v_tex_coord[n*2*2+1]=1.0f-v->tex_coords[0][1];
    v_tex_coord[n*2*2+2]=v->tex_coords[1][0];
    v_tex_coord[n*2*2+3]=1.0f-v->tex_coords[1][1];
  }

  void setVertex( int n,const float coords[3],const float normal[3],const float tex_coords[2][2] ){
    v_coords[n*3+0]=coords[0];v_coords[n*3+1]=coords[1];v_coords[n*3+2]=coords[2];
    v_normal[n*3+0]=-normal[0];v_normal[n*3+1]=-normal[1];v_normal[n*3+2]=-normal[2];
		v_color[n]=0xffffff;
    v_tex_coord[n*2*2+0]=tex_coords[0][0];
    v_tex_coord[n*2*2+1]=1.0f-tex_coords[0][1];
    v_tex_coord[n*2*2+2]=tex_coords[1][0];
    v_tex_coord[n*2*2+3]=1.0f-tex_coords[1][1];
  }

  void setVertex( int n,const float coords[3],const float normal[3],unsigned argb,const float tex_coords[2][2] ){
    v_coords[n*3+0]=coords[0];v_coords[n*3+1]=coords[1];v_coords[n*3+2]=coords[2];
    v_normal[n*3+0]=-normal[0];v_normal[n*3+1]=-normal[1];v_normal[n*3+2]=-normal[2];
		v_color[n]=argb;
		v_tex_coord[n*2*2+0]=tex_coords[0][0];
    v_tex_coord[n*2*2+1]=1.0f-tex_coords[0][1];
    v_tex_coord[n*2*2+2]=tex_coords[1][0];
    v_tex_coord[n*2*2+3]=1.0f-tex_coords[1][1];
  }

  void setTriangle( int n,int v0,int v1,int v2 ){
    tris[n*3+0]=v2;
    tris[n*3+1]=v1;
    tris[n*3+2]=v0;
  }

};

class GLScene : public BBScene{
private:
	bool wireframe;
public:
	GLScene(){
	}

  int  hwTexUnits(){ return 8; }
  int  gfxDriverCaps3D(){ return 0; }

  void setWBuffer( bool enable ){}
  void setHWMultiTex( bool enable ){}
  void setDither( bool enable ){}
  void setAntialias( bool enable ){}
  void setWireframe( bool enable ){
		wireframe=enable;
	}
  void setFlippedTris( bool enable ){
		glFrontFace( enable ? GL_CW : GL_CCW );
	}
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
  void setZMode( int mode ){
		switch( mode ){
		case ZMODE_NORMAL:
			glEnable( GL_DEPTH_TEST );
			glDepthMask( GL_TRUE );
			break;
		case ZMODE_DISABLE:
			glDisable( GL_DEPTH_TEST );
			glDepthMask( GL_TRUE );
			break;
		case ZMODE_CMPONLY:
			glEnable( GL_DEPTH_TEST );
			glDepthMask( GL_FALSE );
			break;
		}
	}
  void setViewport( int x,int y,int w,int h ){
    glViewport( x,y,w,h );
    glScissor( x,y,w,h );
  }
  void setOrthoProj( float nr,float fr,float w,float h ){}
  void setPerspProj( float nr,float fr,float w,float h ){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
    glFrustum( -w/2.0,w/2.0,-h/2.0,h/2.0,nr,fr );
  }
	float view_matrix[16];
  void setViewMatrix( const Matrix *matrix ){
    const Matrix *m=matrix;
    view_matrix[ 0]=m->elements[0][0]; view_matrix[ 1]=m->elements[0][1]; view_matrix[ 2]=m->elements[0][2]; view_matrix[ 3]=0.0f;
    view_matrix[ 4]=m->elements[1][0]; view_matrix[ 5]=m->elements[1][1]; view_matrix[ 6]=m->elements[1][2]; view_matrix[ 7]=0.0f;
    view_matrix[ 8]=m->elements[2][0]; view_matrix[ 9]=m->elements[2][1]; view_matrix[10]=m->elements[2][2]; view_matrix[11]=0.0f;
    view_matrix[12]=m->elements[3][0]; view_matrix[13]=m->elements[3][1]; view_matrix[14]=m->elements[3][2]; view_matrix[15]=1.0f;
  }

  void setWorldMatrix( const Matrix *matrix ){
    const Matrix *m=matrix;

    float mat[16]={
      m->elements[0][0], m->elements[0][1], m->elements[0][2], 0.0f,
      m->elements[1][0], m->elements[1][1], m->elements[1][2], 0.0f,
      m->elements[2][0], m->elements[2][1], m->elements[2][2], 0.0f,
      m->elements[3][0], m->elements[3][1], m->elements[3][2], 1.0f
    };

		glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef( 1.0f,1.0f,-1.0f );
    glMultMatrixf( view_matrix );
    glMultMatrixf( mat );
  }
	void setRenderState( const RenderState &rs ){

		// if( rs.fx&FX_ALPHATEST && !rs.fx&FX_VERTEXALPHA ){
		// 	glEnable( GL_ALPHA_TEST );
		// } else {
		// 	glDisable( GL_ALPHA_TEST );
		// }
		// glEnable( GL_ALPHA_TEST );

		// glDisable( GL_CULL_FACE );
		// glDisable( GL_LIGHTING );

		if( rs.blend==BLEND_REPLACE ){
			glDisable( GL_BLEND );
		} else {
			switch( rs.blend ){
			case BLEND_ALPHA:
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
				break;
			case BLEND_MULTIPLY:
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
				break;
			case BLEND_ADD:
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA,GL_ONE );
				break;
			}
		}

		glShadeModel( rs.fx&FX_FLATSHADED ? GL_FLAT : GL_SMOOTH );

		if( rs.fx&FX_WIREFRAME || wireframe){
			glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
		}else{
			glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
		}

		float mat_ambient[]={ rs.color[0],rs.color[1],rs.color[2],rs.alpha };
		float mat_diffuse[]={ rs.color[0],rs.color[1],rs.color[2],rs.alpha };
		float mat_specular[]={ rs.shininess,rs.shininess,rs.shininess,rs.shininess };
		float mat_shininess[]={ 100.0 };

		glMaterialfv( GL_FRONT,GL_AMBIENT,mat_ambient );
		glMaterialfv( GL_FRONT,GL_DIFFUSE,mat_diffuse );
		glMaterialfv( GL_FRONT,GL_SPECULAR,mat_specular );
		glMaterialfv( GL_FRONT,GL_SHININESS,mat_shininess );

    for( int i=0;i<MAX_TEXTURES;i++ ){
			const RenderState::TexState *ts=&rs.tex_states[i];
      glActiveTexture( GL_TEXTURE0+i );

      GLB2DTextureCanvas *canvas=(GLB2DTextureCanvas*)ts->canvas;

      if( !canvas ){
        glDisable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D,0 );
      } else {
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D,canvas->getTextureId() );

        glMatrixMode( GL_TEXTURE );
				const Matrix *m=ts->matrix;
				if( m ){
					float mat[16]={
						m->elements[0][0], m->elements[0][1], m->elements[0][2], 0.0f,
						m->elements[1][0], m->elements[1][1], m->elements[1][2], 0.0f,
						m->elements[2][0], m->elements[2][1], m->elements[2][2], 0.0f,
						m->elements[3][0], m->elements[3][1], m->elements[3][2], 1.0f
					};
					glLoadMatrixf( mat );
				} else {
					glLoadIdentity();
				}

        if( false ){ // mipmap
          glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
        }else{
          glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
        }

				glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE );
      }
    }
  }

  //rendering
	bool begin( const std::vector<BBLightRep*> &lights ){
		glEnable( GL_SCISSOR_TEST );
		glEnable( GL_CULL_FACE );

		glHint( GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST );

		glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR );
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE );
		glEnable( GL_NORMALIZE );

    glAlphaFunc( GL_GEQUAL,0.5 );

		glClearDepth( 1.0f );
		glDepthFunc( GL_LEQUAL );

		// <temp>
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

		float white_light[4]={ 1.0f,1.0f,1.0f,1.0f };
		glLightfv( GL_LIGHT0,GL_SPECULAR,white_light );

    float z=1.0f,w=0.0f;
    float pos[]={ 0.0,0.0,-z,w };
    float rgba[]={ 1.0f,1.0f,1.0f,1.0f };
    glLightfv( GL_LIGHT0,GL_POSITION,pos );
    glLightfv( GL_LIGHT0,GL_DIFFUSE,rgba );
		// </temp>

		return true;
	}
	void clear( const float rgb[3],float alpha,float z,bool clear_argb,bool clear_z ){
		if( !clear_argb && !clear_z ) return;
		glClearColor( rgb[0],rgb[1],rgb[2],alpha );
		glClearDepth( z );
		glClear( (clear_argb?GL_COLOR_BUFFER_BIT:0)|(clear_z?GL_DEPTH_BUFFER_BIT:0)  );
  }
  void render( BBMesh *m,int first_vert,int vert_cnt,int first_tri,int tri_cnt ){
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

					glNormal3fv( v_normal );
					glTexCoord2fv( v_tex_coord );
					glColor4ub( r,g,b,a );
					glVertex3fv( v_coords );
        }
      }
    glEnd();
  }

  void end(){
  }

  //lighting
  BBLightRep *createLight( int flags ){
    return d_new GLLight();
  }
  void freeLight( BBLightRep *l ){}

  //meshes
  BBMesh *createMesh( int max_verts,int max_tris,int flags ){
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
