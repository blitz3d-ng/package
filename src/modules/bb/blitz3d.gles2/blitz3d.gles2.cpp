
#include "../../stdutil/stdutil.h"
#include <bb/blitz2d.gles2/blitz2d.gles2.h>
#include "blitz3d.gles2.h"

#include "simple.vert.h"
#include "simple.frag.h"

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

		glGenBuffers( 5,vbo );
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
		glBindBuffer( GL_ARRAY_BUFFER,vbo[0] );
		glBufferData( GL_ARRAY_BUFFER,max_verts*3*4,v_coords,GL_STATIC_DRAW );

		glBindBuffer( GL_ARRAY_BUFFER,vbo[1] );
		glBufferData( GL_ARRAY_BUFFER,max_verts*3*4,v_normal,GL_STATIC_DRAW );

		glBindBuffer( GL_ARRAY_BUFFER,vbo[2] );
		glBufferData( GL_ARRAY_BUFFER,max_verts*4,v_color,GL_STATIC_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,vbo[3] );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER,max_tris*3*4,tris,GL_STATIC_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,vbo[4] );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER,max_verts*2*2*4,v_tex_coord,GL_STATIC_DRAW );
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

class GLES2Scene : public BBScene{
private:
	bool wireframe;

	float ambient[4];
	std::vector<GLLight*> lights;

	float view_matrix[16];

	void setLights(){
		// FIXME: replace hardcoded '8' with proper hardward-backed value.
		for( int i=0;i<8;i++ ){
			if( i>=lights.size() ){
				// glDisable( GL_LIGHT0+i );
				continue;
			}

			// glEnable( GL_LIGHT0+i );
			// glPushMatrix();
			// glMultMatrixf( lights[i]->matrix );

			float white_light[4]={ 1.0f,1.0f,1.0f,1.0f };

			// glLightfv( GL_LIGHT0+i,GL_SPECULAR,white_light );

			float z=1.0f,w=0.0f;
			if( lights[i]->type!=Light::LIGHT_DISTANT ){
				z=0.0f;
				w=1.0f;
			}

			float pos[]={ 0.0,0.0,z,w };
			float rgba[]={ lights[i]->r,lights[i]->g,lights[i]->b,1.0f };
			// glLightfv( GL_LIGHT0+i,GL_POSITION,pos );
			// glLightfv( GL_LIGHT0+i,GL_DIFFUSE,rgba );

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
	}

public:
	GLES2Scene():wireframe(false){
		const float MIDLEVEL[]={ 0.5f,0.5f,0.5f,1.0f };
		setAmbient( MIDLEVEL );
		ambient[3]=1.0f;
	}

  int  hwTexUnits(){ return 8; }
  int  gfxDriverCaps3D(){ return 0; }

  void setWBuffer( bool enable ){}
  void setHWMultiTex( bool enable ){}
  void setDither( bool enable ){
		if( enable ){
			// glEnable( GL_DITHER );
		}else{
			// glDisable( GL_DITHER);
		}
	}
  void setAntialias( bool enable ){}
  void setWireframe( bool enable ){
		wireframe=enable;
	}
  void setFlippedTris( bool enable ){
		// glFrontFace( enable ? GL_CW : GL_CCW );
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
			// glEnable( GL_DEPTH_TEST );
			// glDepthMask( GL_TRUE );
			break;
		case ZMODE_DISABLE:
			// glDisable( GL_DEPTH_TEST );
			// glDepthMask( GL_TRUE );
			break;
		case ZMODE_CMPONLY:
			// glEnable( GL_DEPTH_TEST );
			// glDepthMask( GL_FALSE );
			break;
		}
	}
  void setViewport( int x,int y,int w,int h ){
    glViewport( x,y,w,h );
    glScissor( x,y,w,h );
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

		GLint projLocation=glGetUniformLocation( defaultProgram,"bbProjMatrix" );
		glUniformMatrix4fv( projLocation,1,GL_FALSE,projmatrix );
  }

  void setViewMatrix( const Matrix *matrix ){
    const Matrix *m=matrix;
    view_matrix[ 0]=m->elements[0][0]; view_matrix[ 1]=m->elements[0][1]; view_matrix[ 2]=m->elements[0][2]; view_matrix[ 3]=0.0f;
    view_matrix[ 4]=m->elements[1][0]; view_matrix[ 5]=m->elements[1][1]; view_matrix[ 6]=m->elements[1][2]; view_matrix[ 7]=0.0f;
    view_matrix[ 8]=m->elements[2][0]; view_matrix[ 9]=m->elements[2][1]; view_matrix[10]=m->elements[2][2]; view_matrix[11]=0.0f;
    view_matrix[12]=m->elements[3][0]; view_matrix[13]=m->elements[3][1]; view_matrix[14]=m->elements[3][2]; view_matrix[15]=1.0f;

		// glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // glScalef( 1.0f,1.0f,-1.0f );
    // glMultMatrixf( view_matrix );

		GLint viewLocation=glGetUniformLocation( defaultProgram,"bbViewMatrix" );
		glUniformMatrix4fv( viewLocation,1,GL_FALSE,view_matrix );

		setLights();
  }

  void setWorldMatrix( const Matrix *matrix ){
		// glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // glScalef( 1.0f,1.0f,-1.0f );
    // glMultMatrixf( view_matrix );

    if( matrix ){
      const Matrix *m=matrix;

      float mat[16]={
        m->elements[0][0], m->elements[0][1], m->elements[0][2], 0.0f,
        m->elements[1][0], m->elements[1][1], m->elements[1][2], 0.0f,
        m->elements[2][0], m->elements[2][1], m->elements[2][2], 0.0f,
        m->elements[3][0], m->elements[3][1], -m->elements[3][2], 1.0f
      };
      // glMultMatrixf( mat );
			GLint worldLocation=glGetUniformLocation( defaultProgram,"bbWorldMatrix" );
			glUniformMatrix4fv( worldLocation,1,GL_FALSE,mat );
		}
  }
	void setRenderState( const RenderState &rs ){
		if( rs.fx&FX_ALPHATEST && !(rs.fx&FX_VERTEXALPHA) ){
			// glEnable( GL_ALPHA_TEST );
		} else {
			// glDisable( GL_ALPHA_TEST );
		}

		if( rs.blend==BLEND_REPLACE ){
			// glDisable( GL_BLEND );
		} else {
			switch( rs.blend ){
			case BLEND_ALPHA:
				// glEnable( GL_BLEND );
				// glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
				break;
			case BLEND_MULTIPLY:
				// glEnable( GL_BLEND );
				// glBlendFunc( GL_DST_COLOR,GL_ZERO );
				break;
			case BLEND_ADD:
				// glEnable( GL_BLEND );
				// glBlendFunc( GL_SRC_ALPHA,GL_ONE );
				break;
			}
		}

		// glShadeModel( rs.fx&FX_FLATSHADED ? GL_FLAT : GL_SMOOTH );

		if( rs.fx&FX_WIREFRAME || wireframe){
			// glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
		}else{
			// glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
		}

		float mat_ambient[]={ rs.color[0],rs.color[1],rs.color[2],rs.alpha };
		float mat_diffuse[]={ rs.color[0],rs.color[1],rs.color[2],rs.alpha };
		float mat_specular[]={ rs.shininess,rs.shininess,rs.shininess,rs.shininess };
		float mat_shininess[]={ 100.0 };

		// glMaterialfv( GL_FRONT,GL_AMBIENT,mat_ambient );
		// glMaterialfv( GL_FRONT,GL_DIFFUSE,mat_diffuse );
		// glMaterialfv( GL_FRONT,GL_SPECULAR,mat_specular );
		// glMaterialfv( GL_FRONT,GL_SHININESS,mat_shininess );

		if( rs.fx&FX_FULLBRIGHT ){
			const float WHITE[]={ 1.0f,1.0f,1.0f,1.0f };
			// glLightModelfv( GL_LIGHT_MODEL_AMBIENT,WHITE );
		}else{
			// glLightModelfv( GL_LIGHT_MODEL_AMBIENT,ambient );
		}

		if( rs.fx&FX_VERTEXCOLOR ){
			// glEnable( GL_COLOR_MATERIAL );
		}else{
			// glDisable( GL_COLOR_MATERIAL );
		}

    for( int i=0;i<MAX_TEXTURES;i++ ){
			const RenderState::TexState &ts=rs.tex_states[i];
      // glActiveTexture( GL_TEXTURE0+i );

      GLES2B2DTextureCanvas *canvas=(GLES2B2DTextureCanvas*)ts.canvas;

      if( !canvas ){
        // glDisable( GL_TEXTURE_2D );
        // glBindTexture( GL_TEXTURE_2D,0 );
      } else {
				// glEnable( GL_TEXTURE_2D );
				// glBindTexture( GL_TEXTURE_2D,canvas->textureId() );

				// glMatrixMode( GL_TEXTURE );
				const Matrix *m=ts.matrix;
				if( m ){
					float mat[16]={
						m->elements[0][0], m->elements[0][1], m->elements[0][2], 0.0f,
						m->elements[1][0], m->elements[1][1], m->elements[1][2], 0.0f,
						m->elements[2][0], m->elements[2][1], m->elements[2][2], 0.0f,
						m->elements[3][0], m->elements[3][1], m->elements[3][2], 1.0f
					};
					// glLoadMatrixf( mat );
				} else {
					// glLoadIdentity();
				}

				int flags=ts.canvas->getFlags();

				bool no_filter=flags&BBCanvas::CANVAS_TEX_NOFILTERING;
				bool mipmap=flags&BBCanvas::CANVAS_TEX_MIPMAP;

				// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,no_filter?GL_NEAREST:GL_LINEAR );
				// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,mipmap?GL_LINEAR_MIPMAP_LINEAR:(no_filter?GL_NEAREST:GL_LINEAR) );

				if( flags&BBCanvas::CANVAS_TEX_CLAMPU ){
					// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
				} else {
					// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
				}

				if( flags&BBCanvas::CANVAS_TEX_CLAMPV ){
					// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE );
				} else {
					// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );
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
      }
    }
  }

	static GLuint compileShader(GLenum type, const char *shaderName, const unsigned char *shaderData, size_t shaderLength) {
		// FIXME: probably shouldn't hardcode a length here...
		static char shaderString[2048];

		memset( shaderString,0,sizeof(shaderString) );
		memcpy( shaderString,shaderData,shaderLength );

	    // Compile
			const char *shaderSource[1] = { shaderString };
	    GLuint shader = glCreateShader(type);
	    glShaderSource(shader, 1, shaderSource, NULL);
	    glCompileShader(shader);

	    // Check compile status
	    GLint status;
	    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	    if (status == 0) {
	        printf( "Couldn't compile shader: %s\n",shaderName );
	        GLint logLength;
	        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	        if (logLength > 0) {
	            GLchar *log = (GLchar*)malloc(logLength);
	            glGetShaderInfoLog(shader, logLength, &logLength, log);
	            if (log[0] != 0) {
	                printf("Shader log: %s\n", log);
	            }
	            free(log);
	        }
	        glDeleteShader(shader);
	        shader = 0;
	    }
	    return shader;
	}

	GLuint defaultProgram;


	int viewport[4];

  //rendering
	bool begin( const std::vector<BBLightRep*> &l ){
		if( defaultProgram == 0 ){
			GLuint vertShader=compileShader( GL_VERTEX_SHADER,"simple.vert",SIMPLE2_VERT,SIMPLE2_VERT_SIZE );
			GLuint fragShader=compileShader( GL_FRAGMENT_SHADER,"simple.frag",SIMPLE2_FRAG,SIMPLE2_FRAG_SIZE );
			if (vertShader == 0 || fragShader == 0) {
				cout<<"Failed to compile."<<endl;
				exit(1);
			}
			defaultProgram = glCreateProgram();

			glAttachShader( defaultProgram,vertShader );
			glAttachShader( defaultProgram,fragShader );

			glBindAttribLocation( defaultProgram,0,"a_position" );
			glBindAttribLocation( defaultProgram,1,"a_normal" );
			glBindAttribLocation( defaultProgram,2,"a_color" );

			glLinkProgram( defaultProgram );

			glDeleteShader( vertShader );
			glDeleteShader( fragShader );
		}
		glUseProgram( defaultProgram );
		glEnable( GL_SCISSOR_TEST );
		glEnable( GL_CULL_FACE );

		// glEnableClientState( GL_VERTEX_ARRAY );
		// glEnableClientState( GL_COLOR_ARRAY );
		// glEnableClientState( GL_NORMAL_ARRAY );

		// glHint( GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST );

		// glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR );
		// glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE );

		// glEnable( GL_NORMALIZE );

    // glAlphaFunc( GL_GEQUAL,0.5 );

		glClearDepthf( 1.0f );
		glDepthFunc( GL_LEQUAL );

    // glEnable( GL_LIGHTING );

		GLint ambientLocation=glGetUniformLocation( defaultProgram,"bbAmbientLight" );
		glUniform4fv( ambientLocation,1,ambient );

		lights.clear();
		for( int i=0;i<l.size();i++ ) lights.push_back( dynamic_cast<GLLight*>(l[i]) );

		// glGetIntegerv( GL_VIEWPORT,viewport );

		return true;
	}

	void clear( const float rgb[3],float alpha,float z,bool clear_argb,bool clear_z ){
		if( !clear_argb && !clear_z ) return;
		glClearColor( rgb[0],rgb[1],rgb[2],alpha );
		glClearDepthf( z );
		glClear( (clear_argb?GL_COLOR_BUFFER_BIT:0)|(clear_z?GL_DEPTH_BUFFER_BIT:0)  );
	}

	void render( BBMesh *m,int first_vert,int vert_cnt,int first_tri,int tri_cnt ){
		GLMesh *mesh=(GLMesh*)m;

#define PTR(op) reinterpret_cast<void*>(op)

		for( int i=0;i<8;i++ ){
			// glClientActiveTextureARB( GL_TEXTURE0+i );

			// glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			// glBindBufferARB( GL_ARRAY_BUFFER,mesh->vbo[4] );
			// glTexCoordPointer( 2,GL_FLOAT,4*sizeof(float),PTR(first_vert*sizeof(float)*4) );
		}

		cout<<"render..."<<endl;
		glViewport( 0,0,1242,2208 );

		glBindBuffer( GL_ARRAY_BUFFER,mesh->vbo[0] );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,0,PTR(first_vert*sizeof(float)*3) );

		glBindBuffer( GL_ARRAY_BUFFER,mesh->vbo[1] );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1,3,GL_FLOAT,GL_FALSE,0,PTR(first_vert*sizeof(float)*3) );

		glBindBuffer( GL_ARRAY_BUFFER,mesh->vbo[2] );
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 2,4,GL_UNSIGNED_INT,GL_FALSE,0,PTR(first_vert*sizeof(float)*4) );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,mesh->vbo[3] );
		glDrawElements( GL_TRIANGLES,tri_cnt*3,GL_UNSIGNED_INT,PTR(first_tri*3*sizeof(unsigned int)) );
	}

  void end(){
		glUseProgram( 0 );
		// glActiveTexture( GL_TEXTURE0 );
		// glDisable( GL_LIGHTING );
		// glEnable( GL_COLOR_MATERIAL );
		// glDisable( GL_DEPTH_TEST );
		// glMatrixMode( GL_MODELVIEW );
		// glLoadIdentity();
		// glMatrixMode( GL_TEXTURE );
		// glLoadIdentity();

		// glEnable( GL_TEXTURE_2D );
		// glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE );

		// glEnable( GL_BLEND );
		// glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

		// glMatrixMode(GL_PROJECTION);
		// glLoadIdentity();
		// glOrtho( 0.0f,viewport[2],viewport[3],0.0f,-1.0f,1.0f );

		// glViewport( 0,0,viewport[2],viewport[3] );
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

BBScene *GLES2B3DGraphics::createScene( int flags ){
  return d_new GLES2Scene();
}

BBScene *GLES2B3DGraphics::verifyScene( BBScene *scene ){
  return scene;
}

void GLES2B3DGraphics::freeScene( BBScene *scene ){
}

BBMODULE_CREATE( blitz3d_gles2 ){
  return true;
}

BBMODULE_DESTROY( blitz3d_gles2 ){
  return true;
}
