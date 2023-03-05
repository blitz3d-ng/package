
#include "../stdutil/stdutil.h"
#include <bb/blitz2d.gl/blitz2d.gl.h>
#include <bb/system/system.h>
#include "blitz3d.gl.h"
#include "default.glsl.h"

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

struct GLVertex{
	float coords[3];
	float normal[3];
	float color[4];
	float tex_coord[2][2];
};

class GLMesh : public BBMesh{
protected:
	bool dirty()const{ return false; }

public:
	int max_verts,max_tris,flags;
	GLVertex *verts=0;
	unsigned int *tris=0;

	unsigned int vertex_array=0;
	unsigned int vertex_buffer=0,index_buffer=0;

	GLMesh( int mv,int mt,int f ):max_verts(mv),max_tris(mt),flags(f){
		GL( glGenVertexArrays( 1,&vertex_array ) );
		GL( glGenBuffers( 1,&vertex_buffer ) );
		GL( glGenBuffers( 1,&index_buffer ) );

		GL( glBindVertexArray( vertex_array ) );

		GL( glBindBuffer( GL_ARRAY_BUFFER,vertex_buffer ) );
		GL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,index_buffer ) );

		GL( glEnableVertexAttribArray( 0 ) );
		GL( glEnableVertexAttribArray( 1 ) );
		GL( glEnableVertexAttribArray( 2 ) );
		GL( glEnableVertexAttribArray( 3 ) );

		GL( glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,sizeof( GLVertex ),(void*)offsetof( GLVertex,coords ) ) );
		GL( glVertexAttribPointer( 1,3,GL_FLOAT,GL_FALSE,sizeof( GLVertex ),(void*)offsetof( GLVertex,normal ) ) );
		GL( glVertexAttribPointer( 2,4,GL_FLOAT,GL_FALSE,sizeof( GLVertex ),(void*)offsetof( GLVertex,color ) ) );
		GL( glVertexAttribPointer( 3,4,GL_FLOAT,GL_FALSE,sizeof( GLVertex ),(void*)offsetof( GLVertex,tex_coord ) ) );

		GL( glBindVertexArray( 0 ) );
	}

	~GLMesh(){
		delete[] verts;
		delete[] tris;
	}

	bool lock( bool all ){
		// TODO: this should probably come from a pool
		if( !verts ) verts=new GLVertex[max_verts];
		if( !tris ) tris=new unsigned int[max_tris*3];

		return true;
	}

	void unlock(){
		GL( glBindBuffer( GL_ARRAY_BUFFER,vertex_buffer ) );
		GL( glBufferData( GL_ARRAY_BUFFER,max_verts*sizeof(GLVertex),verts,GL_STATIC_DRAW ) );

		GL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,index_buffer ) );
		GL( glBufferData( GL_ELEMENT_ARRAY_BUFFER,max_tris*3*sizeof(unsigned int),tris,GL_STATIC_DRAW ) );
	}

	void setVertex( int n,const void *_v ){
		const Surface::Vertex *v=(const Surface::Vertex*)_v;
		float coords[3]={ v->coords.x,v->coords.y,v->coords.z };
		float normal[3]={ v->normal.x,v->normal.y,v->normal.z };
		setVertex( n,coords,normal,0xffffff,v->tex_coords );
	}

	void setVertex( int n,const float coords[3],const float normal[3],const float tex_coords[2][2] ){
		setVertex( n,coords,normal,0xffffff,tex_coords );
	}

  void setVertex( int n,const float coords[3],const float normal[3],unsigned argb,const float tex_coords[2][2] ){
		verts[n].coords[0]=coords[0];verts[n].coords[1]=coords[1];verts[n].coords[2]=coords[2];
		verts[n].normal[0]=normal[0];verts[n].normal[1]=normal[1];verts[n].normal[2]=normal[2];
		verts[n].tex_coord[0][0]=tex_coords[0][0];verts[n].tex_coord[0][1]=tex_coords[0][1];
		verts[n].tex_coord[1][0]=tex_coords[1][0];verts[n].tex_coord[1][1]=tex_coords[1][1];
		verts[n].color[0]=((argb>>16)&255)/255.0;verts[n].color[1]=((argb>>8)&255)/255.0;verts[n].color[2]=(argb&255)/255.0;verts[n].color[3]=((argb>>24)&255)/255.0;
	}

	void setTriangle( int n,int v0,int v1,int v2 ){
		tris[n*3+0]=v2;
		tris[n*3+1]=v1;
		tris[n*3+2]=v0;
	}
};

// TODO: merge this with GLLight...
struct LightState{
	struct LightData{
		float mat[16];
		float color[4];
	} data[8];

	int lights_used;
};

struct UniformState{
	float ambient[4];
	float brush_color[4];
	float fog_color[4];

	struct GLTexState{
		float mat[16];
		int blend, _0, _1, _2;
	} texs[8];

	float fog_range[2];

	int texs_used;
	int use_vertex_color;
	float brush_shininess;
	int fullbright;
	int fog_mode;
};

class GLScene : public BBScene{
private:
	int context_width,context_height;
	float dpi;
	bool wireframe;
	UniformState us={ 0 };
	GLuint defaultProgram=0;
	int viewport[4];

	std::vector<GLLight*> lights;

	float view_matrix[16];

	void setLights(){
		LightState ls={ 0 };

		// FIXME: replace hardcoded '8' with proper hardward-backed value.
		for( unsigned long i=0;i<8;i++ ){
			if( i>=lights.size() ){
				// glDisable( GL_LIGHT0+i );
				break;
			}

			ls.lights_used++;

			memcpy( ls.data[i].mat,lights[i]->matrix,sizeof(ls.data[i].mat) );
			ls.data[i].color[0]=lights[i]->r;ls.data[i].color[1]=lights[i]->g;ls.data[i].color[2]=lights[i]->b;ls.data[i].color[3]=1.0;

			float z=1.0f,w=0.0f;
			if( lights[i]->type!=Light::LIGHT_DISTANT ){
				z=0.0f;
				w=1.0f;
			}

			float pos[]={ 0.0,0.0,z,w };
			// glLightfv( GL_LIGHT0+i,GL_POSITION,pos );

			if( lights[i]->type!=Light::LIGHT_DISTANT ){
				float light_range[]={ 0.0f };
				float range[]={ lights[i]->range };
				// glLightfv( GL_LIGHT0+i,GL_CONSTANT_ATTENUATION,light_range );
				// glLightfv( GL_LIGHT0+i,GL_LINEAR_ATTENUATION,range );
			}

			if( lights[i]->type==Light::LIGHT_SPOT ){
				float dir[]={ 0.0f,0.0f,-1.0f };
				float outer[]={ lights[i]->outer_angle/2.0f };
				float exponent[]={ 10.0f };
				// glLightfv( GL_LIGHT0+i,GL_SPOT_DIRECTION,dir );
				// glLightfv( GL_LIGHT0+i,GL_SPOT_CUTOFF,outer );
				// glLightfv( GL_LIGHT0+i,GL_SPOT_EXPONENT,exponent );
			}

			// glPopMatrix();
		}

		static unsigned int ubo=0;
		if( ubo==0 ){
			GL( glGenBuffers( 1,&ubo ) );
			GL( glBindBuffer( GL_UNIFORM_BUFFER,ubo ) );
			GL( glBindBufferRange( GL_UNIFORM_BUFFER,1,ubo,0,sizeof(ls) ) );
		}else{
			GL( glBindBuffer( GL_UNIFORM_BUFFER,ubo ) );
		}

		GL( glBufferData( GL_UNIFORM_BUFFER,sizeof(ls),&ls,GL_DYNAMIC_DRAW ) );
		GL( glBindBuffer( GL_UNIFORM_BUFFER,0 ) );
	}

public:
	GLScene( int w,int h,float d ):context_width(w),context_height(h),dpi(d),wireframe(false){
		memset( &us,0,sizeof(UniformState) );

		const float MIDLEVEL[]={ 0.5f,0.5f,0.5f,1.0f };
		setAmbient( MIDLEVEL );
	}

	void resize( int w,int h,float d ){
		context_width=w;context_height=h;
		dpi=d;
	}

	int  hwTexUnits(){ return 8; }
	int  gfxDriverCaps3D(){ return 0; }

	// intentionally left blank...
	void setWBuffer( bool enable ){}
	void setHWMultiTex( bool enable ){}
	void setDither( bool enable ){}

  void setAntialias( bool enable ){}
	void setWireframe( bool enable ){
		wireframe=enable;
	}
	void setFlippedTris( bool enable ){
		GL( glFrontFace( enable ? GL_CW : GL_CCW ) );
	}
	void setAmbient( const float rgb[3] ){
		us.ambient[0]=rgb[0];us.ambient[1]=rgb[1];us.ambient[2]=rgb[2];us.ambient[3]=1.0f;
	}
	void setAmbient2( const float rgb[3] ){
		setAmbient( rgb );
	}
  void setFogColor( const float rgb[3] ){
		us.fog_color[0]=rgb[0];us.fog_color[1]=rgb[1];us.fog_color[2]=rgb[2];us.fog_color[3]=1.0;
	}
  void setFogRange( float nr,float fr ){
		us.fog_range[0]=nr;us.fog_range[1]=fr;
	}
	void setFogMode( int mode ){
		us.fog_mode=mode;
	}
  void setZMode( int mode ){
		switch( mode ){
		case ZMODE_NORMAL:
			GL( glEnable( GL_DEPTH_TEST ) );
			GL( glDepthMask( GL_TRUE ) );
			break;
		case ZMODE_DISABLE:
			GL( glDisable( GL_DEPTH_TEST ) );
			GL( glDepthMask( GL_TRUE ) );
			break;
		case ZMODE_CMPONLY:
			GL( glEnable( GL_DEPTH_TEST ) );
			GL( glDepthMask( GL_FALSE ) );
			break;
		}
	}
	void setViewport( int x,int y,int w,int h ){
		y=context_height-(h+y);

		x*=dpi;y*=dpi;
		w*=dpi;h*=dpi;

		GL( glViewport( x,y,w,h ) );
		GL( glScissor( x,y,w,h ) );
	}
	void setOrthoProj( float nr,float fr,float w,float h ){}

	// lifted from Mesa3D
	void _math_matrix_frustum( float *mat,
		      GLfloat left, GLfloat right,
		      GLfloat bottom, GLfloat top,
		      GLfloat nearval, GLfloat farval )
	{
	   GLfloat x, y, a, b, c, d;

	   x = (2.0F*nearval) / (right-left);
	   y = (2.0F*nearval) / (top-bottom);
	   a = (right+left) / (right-left);
	   b = (top+bottom) / (top-bottom);
	   c = -(farval+nearval) / ( farval-nearval);
	   d = -(2.0F*farval*nearval) / (farval-nearval);  /* error? */

	#define M(row,col)  mat[col*4+row]
	   M(0,0) = x;     M(0,1) = 0.0F;  M(0,2) = a;      M(0,3) = 0.0F;
	   M(1,0) = 0.0F;  M(1,1) = y;     M(1,2) = b;      M(1,3) = 0.0F;
	   M(2,0) = 0.0F;  M(2,1) = 0.0F;  M(2,2) = c;      M(2,3) = d;
	   M(3,0) = 0.0F;  M(3,1) = 0.0F;  M(3,2) = -1.0F;  M(3,3) = 0.0F;
	#undef M
	}

	void setPerspProj( float nr,float fr,float w,float h ){
		float projmatrix[16];
		_math_matrix_frustum( projmatrix,-w/2.0,w/2.0,-h/2.0,h/2.0,nr,fr );

		GLint projLocation=GL( glGetUniformLocation( defaultProgram,"bbProjMatrix" ) );
		GL( glUniformMatrix4fv( projLocation,1,GL_FALSE,projmatrix ) );
	}

	void setViewMatrix( const Matrix *matrix ){
		float mat[16]={
			1.0,0.0, 0.0,0.0,
			0.0,1.0, 0.0,0.0,
			0.0,0.0,-1.0,0.0,
			0.0,0.0, 0.0,1.0
		};

		if( matrix ){
			const Matrix *m=matrix;
			mat[ 0]=m->elements[0][0]; mat[ 1]=m->elements[0][1]; mat[ 2]=-m->elements[0][2];
			mat[ 4]=m->elements[1][0]; mat[ 5]=m->elements[1][1]; mat[ 6]=-m->elements[1][2];
			mat[ 8]=m->elements[2][0]; mat[ 9]=m->elements[2][1]; mat[10]=-m->elements[2][2];
			mat[12]=m->elements[3][0]; mat[13]=m->elements[3][1]; mat[14]=-m->elements[3][2];
		}

		GLint viewLocation=GL( glGetUniformLocation( defaultProgram,"bbViewMatrix" ) );
		GL( glUniformMatrix4fv( viewLocation,1,GL_FALSE,mat ) );
	}

	void setWorldMatrix( const Matrix *matrix ){
		float mat[16]={
			1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0
		};

		if( matrix ){
			const Matrix *m=matrix;
			mat[ 0]=m->elements[0][0];  mat[ 1]=m->elements[0][1]; mat[ 2]=m->elements[0][2];
			mat[ 4]=m->elements[1][0];  mat[ 5]=m->elements[1][1]; mat[ 6]=m->elements[1][2];
			mat[ 8]=m->elements[2][0];  mat[ 9]=m->elements[2][1]; mat[10]=m->elements[2][2];
			mat[12]=m->elements[3][0];  mat[13]=m->elements[3][1]; mat[14]=m->elements[3][2];
		}

		GLint worldLocation=GL( glGetUniformLocation( defaultProgram,"bbWorldMatrix" ) );
		GL( glUniformMatrix4fv( worldLocation,1,GL_FALSE,mat ) );
	}

	void setRenderState( const RenderState &rs ){
		if( rs.fx&FX_ALPHATEST && !(rs.fx&FX_VERTEXALPHA) ){
			// glEnable( GL_ALPHA_TEST );
		} else {
			// glDisable( GL_ALPHA_TEST );
		}

		if( rs.blend==BLEND_REPLACE ){
			GL( glDisable( GL_BLEND ) );
		} else {
			switch( rs.blend ){
			case BLEND_ALPHA:
				GL( glEnable( GL_BLEND ) );
				GL( glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA ) );
				break;
			case BLEND_MULTIPLY:
				GL( glEnable( GL_BLEND ) );
				GL( glBlendFunc( GL_DST_COLOR,GL_ZERO ) );
				break;
			case BLEND_ADD:
				GL( glEnable( GL_BLEND ) );
				GL( glBlendFunc( GL_SRC_ALPHA,GL_ONE ) );
				break;
			}
		}

		// glShadeModel( rs.fx&FX_FLATSHADED ? GL_FLAT : GL_SMOOTH );

		if( rs.fx&FX_WIREFRAME||wireframe ){
			// glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
		}else{
			// glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
		}

		us.brush_color[0]=rs.color[0];us.brush_color[1]=rs.color[1];us.brush_color[2]=rs.color[2];us.brush_color[3]=rs.alpha;
		us.brush_shininess=rs.shininess;

		us.fullbright=rs.fx&FX_FULLBRIGHT;
		us.use_vertex_color=rs.fx&FX_VERTEXCOLOR;

		us.texs_used=0;
		for( int i=0;i<MAX_TEXTURES;i++ ){
			const RenderState::TexState &ts=rs.tex_states[i];
			GL( glActiveTexture( GL_TEXTURE0+i ) );

			GLB2DTextureCanvas *canvas=(GLB2DTextureCanvas*)ts.canvas;

			if( !canvas ){
				GL( glBindTexture( GL_TEXTURE_2D,0 ) );
			} else {
				canvas->bind();

				float mat[16]={
					1.0,0.0, 0.0,0.0,
					0.0,1.0, 0.0,0.0,
					0.0,0.0,-1.0,0.0,
					0.0,0.0, 0.0,1.0
				};

				const Matrix *m=ts.matrix;
				if( m ){
					mat[ 0]=m->elements[0][0]; mat[ 1]=m->elements[0][1]; mat[ 2]=-m->elements[0][2];
					mat[ 4]=m->elements[1][0]; mat[ 5]=m->elements[1][1]; mat[ 6]=-m->elements[1][2];
					mat[ 8]=m->elements[2][0]; mat[ 9]=m->elements[2][1]; mat[10]=-m->elements[2][2];
					mat[12]=m->elements[3][0]; mat[13]=m->elements[3][1]; mat[14]=-m->elements[3][2];
				}

				memcpy( us.texs[us.texs_used].mat,mat,sizeof(mat) );

				int flags=ts.canvas->getFlags();

				bool no_filter=flags&BBCanvas::CANVAS_TEX_NOFILTERING;
				bool mipmap=flags&BBCanvas::CANVAS_TEX_MIPMAP;

				GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,no_filter?GL_NEAREST:GL_LINEAR ) );
				GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,mipmap?GL_LINEAR_MIPMAP_LINEAR:(no_filter?GL_NEAREST:GL_LINEAR) ) );

				if( flags&BBCanvas::CANVAS_TEX_CLAMPU ){
					GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE ) );
				} else {
					GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT ) );
				}

				if( flags&BBCanvas::CANVAS_TEX_CLAMPV ){
					GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE ) );
				} else {
					GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT ) );
				}

				if( flags&BBCanvas::CANVAS_TEX_SPHERE ){
					// glEnable( GL_TEXTURE_GEN_S );
					// glEnable( GL_TEXTURE_GEN_T );
					// glTexGeni( GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP );
					// glTexGeni( GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP );
				}else{
					// glDisable( GL_TEXTURE_GEN_S );
					// glDisable( GL_TEXTURE_GEN_T );
				}

				us.texs[us.texs_used].blend=ts.blend;
				switch( ts.blend ){
				case BLEND_REPLACE:
					// glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
					break;
				case BLEND_ALPHA:
					// glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
					break;
				case BLEND_MULTIPLY:
					// glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
					break;
				// case BLEND_MULTIPLY:
				// 	glTexEnvf( GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,GL_MODULATE );
				// 	break;
				case BLEND_ADD:
					// glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_ADD);
					break;
				case BLEND_DOT3:
					// glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_ARB );
					// glTexEnvf( GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,GL_DOT3_RGB_ARB );
					break;
				case BLEND_MULTIPLY2:
					// glTexEnvi( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE );
					// glTexEnvi( GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_MODULATE );
					// glTexEnvi( GL_TEXTURE_ENV,GL_RGB_SCALE,2.0f );
					break;
				default:
					// glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE );
					break;
				}

				us.texs_used++;
			}
		}

		static unsigned int ubo=0;
		if( ubo==0 ){
			GL( glGenBuffers( 1,&ubo ) );
			GL( glBindBuffer( GL_UNIFORM_BUFFER,ubo ) );
			GL( glBindBufferRange( GL_UNIFORM_BUFFER,2,ubo,0,sizeof(us) ) );
		}else{
			GL( glBindBuffer( GL_UNIFORM_BUFFER,ubo ) );
		}

		GL( glBufferData( GL_UNIFORM_BUFFER,sizeof(us),&us,GL_DYNAMIC_DRAW ) );
		GL( glBindBuffer( GL_UNIFORM_BUFFER,0 ) );
	}

  //rendering
	bool begin( const std::vector<BBLightRep*> &l ){
		if( !glIsProgram( defaultProgram ) ){
			GL( glUseProgram( 0 ) );

			// _bbLog( "rebuilding shader...\n" );

			string src( DEFAULT_GLSL,DEFAULT_GLSL+DEFAULT_GLSL_SIZE );
			defaultProgram=_bbGLCompileProgram( "default.glsl",src );
			if( !defaultProgram ){
				abort();
			}

			GL( glUseProgram( defaultProgram ) );

			for( int i=0;i<MAX_TEXTURES;i++ ){
				char sampler_name[20];
				sprintf( sampler_name,"bbTexture[%i]",i );
				GLint texLocation=GL( glGetUniformLocation( defaultProgram,sampler_name ) );
				GL( glUniform1i( texLocation,i ) );
			}

			GLint lightIdx=GL( glGetUniformBlockIndex( defaultProgram,"BBLightState" ) );
			GL( glUniformBlockBinding( defaultProgram,lightIdx,1 ) );

			GLint renderIdx=GL( glGetUniformBlockIndex( defaultProgram,"BBRenderState" ) );
			GL( glUniformBlockBinding( defaultProgram,renderIdx,2 ) );
		}

		GL( glUseProgram( defaultProgram ) );

		GL( glEnable( GL_SCISSOR_TEST ) );
		GL( glEnable( GL_CULL_FACE ) );

		// glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR );
		// glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE );

    // glAlphaFunc( GL_GEQUAL,0.5 );

		GL( glDepthFunc( GL_LEQUAL ) );

		lights.clear();
		for( unsigned long i=0;i<l.size();i++ ) lights.push_back( dynamic_cast<GLLight*>(l[i]) );

		setLights();

		GL( glGetIntegerv( GL_VIEWPORT,viewport ) );

		return true;
	}

	void clear( const float rgb[3],float alpha,float z,bool clear_argb,bool clear_z ){
		if( !clear_argb && !clear_z ) return;

		GL( glClearColor( rgb[0],rgb[1],rgb[2],alpha ) );
		GL( glClearDepthf( z ) );
		GL( glClear( (clear_argb?GL_COLOR_BUFFER_BIT:0)|(clear_z?GL_DEPTH_BUFFER_BIT:0)  ) );
	}

	void render( BBMesh *m,int first_vert,int vert_cnt,int first_tri,int tri_cnt ){
		GLMesh *mesh=(GLMesh*)m;

		GL( glBindVertexArray( mesh->vertex_array ) );
		GL( glDrawRangeElements( GL_TRIANGLES,first_tri,tri_cnt*3,tri_cnt*3,GL_UNSIGNED_INT,0 ) );
		GL( glBindVertexArray( 0 ) );
	}

	void end(){
		GL( glUseProgram( 0 ) );
		GL( glActiveTexture( GL_TEXTURE0 ) );
		GL( glDisable( GL_DEPTH_TEST ) );

		GL( glDisable( GL_BLEND ) );

		GL( glViewport( viewport[0],viewport[1],viewport[2],viewport[3] ) );
		GL( glScissor( viewport[0],viewport[1],viewport[2],viewport[3] ) );
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

static GLScene *scene=0; // TODO: I don't like this...consider a better approach...

BBScene *GLB3DGraphics::createScene( int w,int h,float d,int flags ){
  return (scene=d_new GLScene( w,h,d ));
}

BBScene *GLB3DGraphics::verifyScene( BBScene *scene ){
  return scene;
}

void GLB3DGraphics::freeScene( BBScene *scene ){
}

void GLB3DGraphics::resize( int w,int h,float dpi ){
	if( scene ){
		scene->resize( w,h,dpi );
	}
}

BBMODULE_CREATE( blitz3d_gl ){
	return true;
}

BBMODULE_DESTROY( blitz3d_gl ){
	return true;
}
