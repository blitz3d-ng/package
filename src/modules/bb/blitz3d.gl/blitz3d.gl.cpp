
#include "../../stdutil/stdutil.h"
#include <bb/blitz2d.gl/blitz2d.gl.h>
#include "blitz3d.gl.h"

#include <iostream>
using namespace std;

//degrees to radians and back
static const float dtor=0.0174532925199432957692369076848861f;
static const float rtod=1/dtor;


class GLLight : public BBLightRep{
public:
	int type;

	float r,g,b;
	float matrix[16];

	float range;
	float inner_angle,outer_angle;

  void update( Light *light ){
		type=light->getType();

		r=light->getColor().x;
		g=light->getColor().y;
		b=light->getColor().z;

		range=light->getRange();
		light->getConeAngles( inner_angle,outer_angle );
		outer_angle*=rtod;

		const BBScene::Matrix *m=(BBScene::Matrix*)&(light->getRenderTform());

		matrix[ 0]=m->elements[0][0]; matrix[ 1]=m->elements[0][1]; matrix[ 2]=m->elements[0][2]; matrix[ 3]=0.0f;
		matrix[ 4]=m->elements[1][0]; matrix[ 5]=m->elements[1][1]; matrix[ 6]=m->elements[1][2]; matrix[ 7]=0.0f;
		matrix[ 8]=m->elements[2][0]; matrix[ 9]=m->elements[2][1]; matrix[10]=m->elements[2][2]; matrix[11]=0.0f;
		matrix[12]=m->elements[3][0]; matrix[13]=m->elements[3][1]; matrix[14]=m->elements[3][2]; matrix[15]=1.0f;
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

	unsigned int vbo[12];

  GLMesh( int mv,int mt,int f ):max_verts(mv),max_tris(mt),flags(f){
    v_coords=new float[max_verts*3];
    v_normal=new float[max_verts*3];
    v_tex_coord=new float[max_verts*2*2];
    v_color=new unsigned int[max_verts];
    tris=new int[max_tris*3];

		glGenBuffersARB( 5,vbo );
  }

  ~GLMesh(){
    delete[] v_coords;
    delete[] v_normal;
    delete[] v_tex_coord;
    delete[] v_color;
    delete[] tris;
  }

  bool lock( bool all ){
		return true;
  }

  void unlock(){
		glBindBufferARB( GL_ARRAY_BUFFER_ARB,vbo[0] );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB,max_verts*3*4,v_coords,GL_STATIC_DRAW_ARB );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB,vbo[1] );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB,max_verts*3*4,v_normal,GL_STATIC_DRAW_ARB );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB,vbo[2] );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB,max_verts*4,v_color,GL_STATIC_DRAW_ARB );

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB,vbo[3] );
		glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,max_tris*3*4,tris,GL_STATIC_DRAW_ARB );

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB,vbo[4] );
		glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,max_verts*2*2*4,v_tex_coord,GL_STATIC_DRAW_ARB );

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

	float ambient[4];
	std::vector<GLLight*> lights;

	float view_matrix[16];

	int render_w,render_h;

	void setLights(){
		// FIXME: replace hardcoded '8' with proper hardward-backed value.
		for( int i=0;i<8;i++ ){
			if( i>=lights.size() ){
				glDisable( GL_LIGHT0+i );
				continue;
			}

			glEnable( GL_LIGHT0+i );
			glPushMatrix();
			glMultMatrixf( lights[i]->matrix );

			float white_light[4]={ 1.0f,1.0f,1.0f,1.0f };
			glLightfv( GL_LIGHT0+i,GL_SPECULAR,white_light );

			float z=1.0f,w=0.0f;
			if( lights[i]->type!=Light::LIGHT_DISTANT ){
				z=0.0f;
				w=1.0f;
			}

			float pos[]={ 0.0,0.0,z,w };
			float rgba[]={ lights[i]->r,lights[i]->g,lights[i]->b,1.0f };
			glLightfv( GL_LIGHT0+i,GL_POSITION,pos );
			glLightfv( GL_LIGHT0+i,GL_DIFFUSE,rgba );

			if( lights[i]->type!=Light::LIGHT_DISTANT ){
				float light_range[]={ 0.0f };
				float range[]={ lights[i]->range };
				glLightfv( GL_LIGHT0+i,GL_CONSTANT_ATTENUATION,light_range );
				glLightfv( GL_LIGHT0+i,GL_LINEAR_ATTENUATION,range );
			}

			if( lights[i]->type==Light::LIGHT_SPOT ){
				float dir[]={ 0.0f,0.0f,-1.0f };
				float outer[]={ lights[i]->outer_angle/2.0f };
				float exponent[]={ 10.0f };
				glLightfv( GL_LIGHT0+i,GL_SPOT_DIRECTION,dir );
				glLightfv( GL_LIGHT0+i,GL_SPOT_CUTOFF,outer );
				glLightfv( GL_LIGHT0+i,GL_SPOT_EXPONENT,exponent );
			}

			glPopMatrix();
		}
	}

public:
	GLScene( int w,int h ):wireframe(false){
		const float MIDLEVEL[]={ 0.5f,0.5f,0.5f,1.0f };
		setAmbient( MIDLEVEL );
		ambient[3]=1.0f;
		render_w=w;render_h=h;
	}

  int  hwTexUnits(){ return 8; }
  int  gfxDriverCaps3D(){ return 0; }

  void setWBuffer( bool enable ){}
  void setHWMultiTex( bool enable ){}
  void setDither( bool enable ){
		if( enable ){
			glEnable( GL_DITHER );
		}else{
			glDisable( GL_DITHER);
		}
	}
  void setAntialias( bool enable ){}
  void setWireframe( bool enable ){
		wireframe=enable;
	}
  void setFlippedTris( bool enable ){
		glFrontFace( enable ? GL_CW : GL_CCW );
	}
  void setAmbient( const float rgb[3] ){
		memcpy( ambient,rgb,sizeof(float)*3 );
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
    glViewport( x,render_h-h-y,w,h );
    glScissor( x,render_h-h-y,w,h );
  }
  void setOrthoProj( float nr,float fr,float w,float h ){}
  void setPerspProj( float nr,float fr,float w,float h ){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
    glFrustum( -w/2.0,w/2.0,-h/2.0,h/2.0,nr,fr );
  }

  void setViewMatrix( const Matrix *matrix ){
    const Matrix *m=matrix;
    view_matrix[ 0]=m->elements[0][0]; view_matrix[ 1]=m->elements[0][1]; view_matrix[ 2]=m->elements[0][2]; view_matrix[ 3]=0.0f;
    view_matrix[ 4]=m->elements[1][0]; view_matrix[ 5]=m->elements[1][1]; view_matrix[ 6]=m->elements[1][2]; view_matrix[ 7]=0.0f;
    view_matrix[ 8]=m->elements[2][0]; view_matrix[ 9]=m->elements[2][1]; view_matrix[10]=m->elements[2][2]; view_matrix[11]=0.0f;
    view_matrix[12]=m->elements[3][0]; view_matrix[13]=m->elements[3][1]; view_matrix[14]=m->elements[3][2]; view_matrix[15]=1.0f;

		glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef( 1.0f,1.0f,-1.0f );
    glMultMatrixf( view_matrix );

		setLights();
  }

  void setWorldMatrix( const Matrix *matrix ){
		glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef( 1.0f,1.0f,-1.0f );
    glMultMatrixf( view_matrix );

    if( matrix ){
      const Matrix *m=matrix;

      float mat[16]={
        m->elements[0][0], m->elements[0][1], m->elements[0][2], 0.0f,
        m->elements[1][0], m->elements[1][1], m->elements[1][2], 0.0f,
        m->elements[2][0], m->elements[2][1], m->elements[2][2], 0.0f,
        m->elements[3][0], m->elements[3][1], m->elements[3][2], 1.0f
      };

      glMultMatrixf( mat );
    }
  }
	void setRenderState( const RenderState &rs ){

		if( rs.fx&FX_ALPHATEST && !(rs.fx&FX_VERTEXALPHA) ){
			glEnable( GL_ALPHA_TEST );
		} else {
			glDisable( GL_ALPHA_TEST );
		}

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
				glBlendFunc( GL_DST_COLOR,GL_ZERO );
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

		if( rs.fx&FX_FULLBRIGHT ){
			const float WHITE[]={ 1.0f,1.0f,1.0f,1.0f };
			glLightModelfv( GL_LIGHT_MODEL_AMBIENT,WHITE );
		}else{
			glLightModelfv( GL_LIGHT_MODEL_AMBIENT,ambient );
		}

		if( rs.fx&FX_VERTEXCOLOR ){
			glEnable( GL_COLOR_MATERIAL );
		}else{
			glDisable( GL_COLOR_MATERIAL );
		}

    for( int i=0;i<MAX_TEXTURES;i++ ){
			const RenderState::TexState &ts=rs.tex_states[i];
      glActiveTexture( GL_TEXTURE0+i );

      GLB2DTextureCanvas *canvas=(GLB2DTextureCanvas*)ts.canvas;

      if( !canvas ){
        glDisable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D,0 );
      } else {
				glEnable( GL_TEXTURE_2D );
				glBindTexture( GL_TEXTURE_2D,canvas->textureId() );

				glMatrixMode( GL_TEXTURE );
				const Matrix *m=ts.matrix;
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

				int flags=ts.canvas->getFlags();

				bool no_filter=flags&BBCanvas::CANVAS_TEX_NOFILTERING;
				bool mipmap=flags&BBCanvas::CANVAS_TEX_MIPMAP;

				glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,no_filter?GL_NEAREST:GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,mipmap?GL_LINEAR_MIPMAP_LINEAR:(no_filter?GL_NEAREST:GL_LINEAR) );

				if( flags&BBCanvas::CANVAS_TEX_CLAMPU ){
					glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
				} else {
					glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
				}

				if( flags&BBCanvas::CANVAS_TEX_CLAMPV ){
					glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE );
				} else {
					glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );
				}

				if( flags&BBCanvas::CANVAS_TEX_SPHERE ){
					glEnable( GL_TEXTURE_GEN_S );
					glEnable( GL_TEXTURE_GEN_T );
					glTexGeni( GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP );
					glTexGeni( GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP );
				}else{
					glDisable( GL_TEXTURE_GEN_S );
					glDisable( GL_TEXTURE_GEN_T );
				}

				switch( ts.blend ){
				case BLEND_REPLACE:
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
					break;
				case BLEND_ALPHA:
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
					break;
				case BLEND_MULTIPLY:
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
					break;
				// case BLEND_MULTIPLY:
				// 	glTexEnvf( GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,GL_MODULATE );
				// 	break;
				case BLEND_ADD:
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_ADD);
					break;
				case BLEND_DOT3:
					glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_ARB );
					glTexEnvf( GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,GL_DOT3_RGB_ARB );
					break;
				case BLEND_MULTIPLY2:
					glTexEnvi( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE );
					glTexEnvi( GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_MODULATE );
					glTexEnvi( GL_TEXTURE_ENV,GL_RGB_SCALE,2.0f );
					break;
				default:
					glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE );
					break;
				}
      }
    }
  }

	int viewport[4];

  //rendering
	bool begin( const std::vector<BBLightRep*> &l ){
		glEnable( GL_SCISSOR_TEST );
		glEnable( GL_CULL_FACE );

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );

		glHint( GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST );

// FIXME: find out why it's not available under emscripten
#ifndef __EMSCRIPTEN__
		glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR );
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE );
#endif
		glEnable( GL_NORMALIZE );

    glAlphaFunc( GL_GEQUAL,0.5 );

		glClearDepth( 1.0f );
		glDepthFunc( GL_LEQUAL );

    glEnable( GL_LIGHTING );

		lights.clear();
		for( int i=0;i<l.size();i++ ) lights.push_back( dynamic_cast<GLLight*>(l[i]) );

		glGetIntegerv( GL_VIEWPORT,viewport );

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

#define PTR(op) reinterpret_cast<void*>(op)

		for( int i=0;i<8;i++ ){
			glClientActiveTextureARB( GL_TEXTURE0+i );

			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB,mesh->vbo[4] );
			glTexCoordPointer( 2,GL_FLOAT,4*sizeof(float),PTR(first_vert*sizeof(float)*4) );
		}

		glBindBufferARB( GL_ARRAY_BUFFER_ARB,mesh->vbo[0] );
		glVertexPointer( 3,GL_FLOAT,0,PTR(first_vert*sizeof(float)*3) );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB,mesh->vbo[1] );
		glNormalPointer( GL_FLOAT,0,PTR(first_vert*sizeof(float)*3) );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB,mesh->vbo[2] );
		glColorPointer( 4,GL_UNSIGNED_INT,0,PTR(first_vert*sizeof(float)*4) );

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB,mesh->vbo[3] );
		glDrawElements( GL_TRIANGLES,tri_cnt*3,GL_UNSIGNED_INT,PTR(first_tri*3*sizeof(unsigned int)) );
	}

  void end(){
		glActiveTexture( GL_TEXTURE0 );
		glDisable( GL_LIGHTING );
		glEnable( GL_COLOR_MATERIAL );
		glDisable( GL_DEPTH_TEST );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glMatrixMode( GL_TEXTURE );
		glLoadIdentity();

		glEnable( GL_TEXTURE_2D );
		glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho( viewport[0],viewport[0]+viewport[2],viewport[1]+viewport[3],viewport[1],-1.0f,1.0f );

		glViewport( viewport[0],viewport[1],viewport[2],viewport[3] );
		glScissor( viewport[0],viewport[1],viewport[2],viewport[3] );
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

BBScene *GLB3DGraphics::createScene( int w,int h,int flags ){
  return d_new GLScene( w,h );
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
