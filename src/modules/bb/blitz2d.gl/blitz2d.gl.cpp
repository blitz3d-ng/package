
#include "../stdutil/stdutil.h"
#include <bb/blitz2d/font.h>
#include "blitz2d.gl.h"
#include <cmath>
#include <map>
#include <algorithm>
using namespace std;

#include "default.glsl.h"

map<BBImageFont*,unsigned int> font_textures;

struct UniformState{
	float xywh[4];
	float res[2];
	float texscale[2];
	float color[3];
	int texenabled;
	float scale;
};

struct Vertex{
	float x,y;
	float u,v;
};

static GLuint defaultProgram=0;

static
bool makeProgram( float dpi,float tx,float ty,bool tex_enabled,float resx,float resy,float x,float y,float width,float height,float color[3] ){
	if( !glIsProgram( defaultProgram ) ){
		// _bbLog( "rebuilding 2d shader...\n" );

		string src( DEFAULT_GLSL,DEFAULT_GLSL+DEFAULT_GLSL_SIZE );
		defaultProgram=GL( _bbGLCompileProgram( "default.glsl",src ) );
		if( !defaultProgram ){
			return false;
		}

		GL( glUseProgram( defaultProgram ) );

		GLint texLocation=GL( glGetUniformLocation( defaultProgram,"u_tex" ) );
		GL( glUniform1i( texLocation,0 ) );

		GLint idx=GL( glGetUniformBlockIndex( defaultProgram,"BBRenderState" ) );
		GL( glUniformBlockBinding( defaultProgram,idx,0 ) );
	}

	GL( glUseProgram( defaultProgram ) );

	UniformState us={ 0 };
	us.texscale[0]=tx;us.texscale[1]=ty;
	us.texenabled=tex_enabled;
	us.res[0]=resx*dpi;us.res[1]=resy*dpi;
	us.xywh[0]=x;us.xywh[1]=y;us.xywh[2]=width;us.xywh[3]=height;
	us.color[0]=color[0];us.color[1]=color[1];us.color[2]=color[2];
	us.scale=dpi;

	static unsigned int ubo=0;
	if( !ubo ){
		GL( glGenBuffers( 1,&ubo ) );
		GL( glBindBuffer( GL_UNIFORM_BUFFER,ubo ) );
		GL( glBindBufferRange( GL_UNIFORM_BUFFER,0,ubo,0,sizeof(us) ) );
	}else{
		GL( glBindBuffer( GL_UNIFORM_BUFFER,ubo ) );
	}

	GL( glBufferData( GL_UNIFORM_BUFFER,sizeof(us),&us,GL_DYNAMIC_DRAW ) );
	GL( glBindBuffer( GL_UNIFORM_BUFFER,0 ) );

	return true;
}

static
void initArrays( int size,GLuint* buffer,GLuint *array ){
	if( !glIsVertexArray( buffer[0] ) ){
		GL( glGenVertexArrays( size,array ) );
		GL( glGenBuffers( size,buffer ) );

		for( int i=0;i<size;i++) {
			GL( glBindVertexArray( array[i] ) );
			GL( glBindBuffer( GL_ARRAY_BUFFER,buffer[i] ) );

			GL( glEnableVertexAttribArray( 0 ) );
			GL( glEnableVertexAttribArray( 1 ) );

			GL( glVertexAttribPointer( 0,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),0 ) );
			GL( glVertexAttribPointer( 1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof( Vertex,u ) ) );
		}
	}
}

void GLB2DCanvas::setFont( BBFont *f ){
	font=reinterpret_cast<BBImageFont*>(f);
}

void GLB2DCanvas::setMask( unsigned argb ){
}

void GLB2DCanvas::setColor( unsigned argb ){
	color[0]=((argb>>16)&255)/255.0f;
	color[1]=((argb>>8)&255)/255.0f;;
	color[2]=(argb&255)/255.0f;;
}

void GLB2DCanvas::setClsColor( unsigned argb ){
	int r = (argb >> 16) & 255;
	int g = (argb >> 8) & 255;
	int b = argb & 255;

	GL( glClearColor( r/255.0f,g/255.0f,b/255.0f,1.0f ) );
}

void GLB2DCanvas::setOrigin( int x,int y ){
	origin_x=x;
	origin_y=y;
}

void GLB2DCanvas::setHandle( int x,int y ){
	handle_x=x;
	handle_y=y;
}

void GLB2DCanvas::setViewport( int x,int y,int w,int h ){
	x*=dpi;y*=dpi;
	w*=dpi;h*=dpi;

	GL( glEnable( GL_SCISSOR_TEST ) );
	GL( glViewport( x,y,w,h ) );
	GL( glScissor( x,y,w,h ) );
}

void GLB2DCanvas::cls(){
	GL( glClear( GL_COLOR_BUFFER_BIT ) );
}

void GLB2DCanvas::plot( int x,int y ){
	static GLuint buffer=0,array=0;
	if( buffer==0 ){
		const Vertex vertices[1] = {
			{ 0.0,0.0,0.0,0.0 }
		};

		initArrays( 1,&buffer,&array );
		GL( glBindBuffer( GL_ARRAY_BUFFER,buffer ) );
		GL( glBufferData( GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW ) );
	}

	// GL( glEnable(GL_PROGRAM_POINT_SIZE) );

	makeProgram( dpi,1.0,1.0,false,width,height,x,y,1.0,1.0,color );

	GL( glBindVertexArray( array ) );
	GL( glDrawArrays( GL_POINTS,0,1 ) );
	GL( glBindVertexArray( 0 ) );
}

void GLB2DCanvas::line( int x,int y,int x2,int y2 ){
	const Vertex vertices[2] = {
		{ (float)x,(float)y,0.0,0.0 },
		{ (float)x2,(float)y2,0.0,0.0 }
	};

	static GLuint buffer=0,array=0;
	initArrays( 1,&buffer,&array );

	GL( glBindBuffer( GL_ARRAY_BUFFER,buffer ) );
	GL( glBufferData( GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW ) );

	makeProgram( dpi,1.0,1.0,false,width,height,0.0,0.0,1.0,1.0,color );

	GL( glBindVertexArray( array ) );
	GL( glDrawArrays( GL_LINES,0,2 ) );
	GL( glBindVertexArray( 0 ) );
}

void GLB2DCanvas::rect( int x,int y,int w,int h,bool solid ){
	quad( x,y,w,h,solid,false,1.0,1.0,color );
}

void GLB2DCanvas::quad( int x,int y,int w,int h,bool solid,bool texenabled,float tx,float ty,float color[3] ){
	static const Vertex vertices[2][4] = {
		{
			{ 0.0, 0.0, 0.0, 0.0 },
			{ 0.0, 1.0, 0.0, 1.0 },
			{ 1.0, 0.0, 1.0, 0.0 },
			{ 1.0, 1.0, 1.0, 1.0 }
		},
		{
			{ 0.0, 0.0, 0.0, 0.0 },
			{ 0.0, 1.0, 0.0, 1.0 },
			{ 1.0, 1.0, 1.0, 1.0 },
			{ 1.0, 0.0, 1.0, 0.0 }
		}
	};

	static GLuint buffer[2]={ 0,0 },array[2]={ 0,0 };
	if( buffer[0]==0 ){
		initArrays( 2,buffer,array );

		GL( glBindBuffer( GL_ARRAY_BUFFER,buffer[0] ) );
		GL( glBufferData( GL_ARRAY_BUFFER,sizeof(vertices[0]),vertices[0],GL_STATIC_DRAW ) );

		GL( glBindBuffer( GL_ARRAY_BUFFER,buffer[1] ) );
		GL( glBufferData( GL_ARRAY_BUFFER,sizeof(vertices[1]),vertices[1],GL_STATIC_DRAW ) );

		GL( glBindBuffer( GL_ARRAY_BUFFER,0 ) );
	}

	makeProgram( dpi,tx,ty,texenabled,width,height,x,y,w,h,color );

	int i=solid?0:1;
	GL( glBindVertexArray( array[i] ) );
	GL( glDrawArrays( solid?GL_TRIANGLE_STRIP:GL_LINE_LOOP,0,4 ) );
	GL( glBindVertexArray( 0 ) );
}

void GLB2DCanvas::oval( int x,int y,int w,int h,bool solid ){
	int rx=w/2.0f,ry=h/2.0f;
	int segs=abs(rx)+abs(ry);

	float theta = 2*3.1415926/float(segs);
	float c=cosf(theta),s=sinf(theta),t;

	x+=rx;y+=ry;

	float vx=1.0f,vy=0.0f;

	vector<Vertex> verts;
	for( int i=0;i<segs;i++ ){
		verts.push_back( { vx*rx+x,vy*ry+y,0.0,0.0 } );

		t=vx;
		vx=c*vx-s*vy;
		vy=s*t+c*vy;
	}
	std::reverse( verts.begin(),verts.end() );

	static GLuint buffer=0,array=0;
	if( buffer==0 ){
		initArrays( 1,&buffer,&array );
	}

	makeProgram( dpi,1.0,1.0,false,width,height,0.0,0.0,1.0,1.0,color );

	GL( glBindBuffer( GL_ARRAY_BUFFER,buffer ) );
	GL( glBufferData( GL_ARRAY_BUFFER,sizeof(Vertex)*verts.size(),verts.data(),GL_DYNAMIC_DRAW ) );

	GL( glBindVertexArray( array ) );
	GL( glDrawArrays( solid?GL_TRIANGLE_FAN:GL_LINE_LOOP,0,verts.size() ) );
	GL( glBindVertexArray( 0 ) );
}

void GLB2DCanvas::text( int x,int y,const std::string &t ){
	if( !font ) return;

	unsigned int texture;
	if( font_textures.count(font)==0 ){
		GL( glGenTextures( 1,&texture ) );
		font_textures[font]=texture;
	}else{
		texture=font_textures[font];
	}

	GL( glActiveTexture( GL_TEXTURE0 ) );
	GL( glBindTexture( GL_TEXTURE_2D,texture ) );

	if( font->loadChars( t ) ){
		font->rebuildAtlas();

		int size=font->atlas->width*font->atlas->height;
		vector<char> bmp( size*4 );
		for( int i=0;i<font->atlas->width*font->atlas->height;i++ ){
			bmp[i*4+0]=font->atlas->bits[i];
			bmp[i*4+1]=font->atlas->bits[i];
			bmp[i*4+2]=font->atlas->bits[i];
			bmp[i*4+3]=font->atlas->bits[i];
		}

		GL( glPixelStorei( GL_UNPACK_ALIGNMENT,1 ) );
		GL( glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,font->atlas->width,font->atlas->height,0,GL_RGBA,GL_UNSIGNED_BYTE,bmp.data() ) );
		GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE ) );
		GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE ) );
		GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR ) );
		GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR ) );
	}

	vector<Vertex> verts;

	y+=font->baseline;

	for( int i=0;i<t.length();i++ ){
		BBImageFont::Char c=font->getChar( t[i] );

		int cx=x+c.bearing_x,cy=y-c.bearing_y;

		if( i>0 ){
			cx+=font->getKerning( t[i-1],t[i] );
		}

		float l=c.x/(float)font->atlas->width;
		float r=(c.x+c.width)/(float)font->atlas->width;
		float t=c.y/(float)font->atlas->height;
		float b=(c.y+c.height)/(float)font->atlas->height;

		Vertex lb={ (float)cx,(float)cy+c.height,l,b },
		       rb={ (float)cx+c.width,(float)cy+c.height,r,b },
		       rt={ (float)cx+c.width,(float)cy,r,t },
		       lt={ (float)cx,(float)cy,l,t };

		verts.push_back( lb );verts.push_back( rb );verts.push_back( rt );
		verts.push_back( lb );verts.push_back( rt );verts.push_back( lt );

		x+=c.advance;
	}

	GL( glEnable( GL_BLEND ) );
	GL( glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA ) );

	static GLuint buffer=0,array=0;
	if( buffer==0 ){
		initArrays( 1,&buffer,&array );
	}

	makeProgram( dpi,1.0,1.0,true,width,height,0.0,0.0,1.0,1.0,color );

	GL( glBindBuffer( GL_ARRAY_BUFFER,buffer ) );
	GL( glBufferData( GL_ARRAY_BUFFER,sizeof(Vertex)*verts.size(),verts.data(),GL_DYNAMIC_DRAW ) );
	GL( glBindBuffer( GL_ARRAY_BUFFER,0 ) );

	GL( glBindVertexArray( array ) );
	GL( glDrawArrays( GL_TRIANGLES,0,verts.size() ) );
	GL( glBindVertexArray( 0 ) );

	GL( glDisable( GL_BLEND ) );
}

void GLB2DCanvas::blit( int x,int y,BBCanvas *s,int src_x,int src_y,int src_w,int src_h,bool solid ){
	unsigned int cfb;
	GL( glGetIntegerv( GL_FRAMEBUFFER_BINDING,(GLint*)&cfb ) );

	GLB2DCanvas *src=(GLB2DCanvas*)s;

	unsigned int rfb=src->framebufferId(),dfb=framebufferId();

	GL( glBindFramebuffer( GL_READ_FRAMEBUFFER,rfb ) );
	GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER,dfb ) );
	GL( glBlitFramebuffer( src_x,src_y,src_w,src_h,x,y,src_w,src_h,GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST ) );

	GL( glBindFramebuffer( GL_FRAMEBUFFER,cfb ) );
}

void GLB2DCanvas::image( BBCanvas *c,int x,int y,bool solid ){
	GLB2DTextureCanvas *src=(GLB2DTextureCanvas*)c;

	src->bind();

	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE ) );

	float white[3]={ 1.0,1.0,1.0 };
	quad( x-src->handle_x,y-src->handle_y,src->getWidth(),src->getHeight(),true,true,1.0,1.0,white );
}

void GLB2DCanvas::setPixel( int x,int y,unsigned argb ){
	setPixelFast( x,y,argb );
}

#define UC(c) static_cast<unsigned char>(c)

void GLB2DCanvas::setPixelFast( int x,int y,unsigned argb ){
	set();

	// unsigned char rgba[4]={ UC((argb>>16)&255),UC((argb>>8)&255),UC(argb&255),UC((argb>>24)&255) };
	// glRasterPos2f( x,y+1 );
	// glDrawPixels( 1,1,GL_RGBA,GL_UNSIGNED_BYTE,rgba );
}

// --- GLB2DDefaultCanvas ---

void GLB2DDefaultCanvas::bind()const{
}

GLB2DDefaultCanvas::GLB2DDefaultCanvas( unsigned int fb,int m,int f ):GLB2DCanvas(f),framebuffer(fb),mode(m){
	set();
	GL( glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) );
}

void GLB2DDefaultCanvas::unset(){
	GL( glFlush() );
}

void GLB2DDefaultCanvas::set(){
	GL( glBindFramebuffer( GL_FRAMEBUFFER,framebuffer ) );

#ifdef BB_DESKTOP
	if( framebuffer==0 ){
		GL( glDrawBuffer( mode ) );
	}
#endif

	GL( glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) );
}

unsigned int GLB2DDefaultCanvas::framebufferId(){
	return framebuffer;
}

void GLB2DDefaultCanvas::blit( int x,int y,BBCanvas *src,int src_x,int src_y,int src_w,int src_h,bool solid ){
}

int GLB2DDefaultCanvas::getDepth()const{
	return 8;
}

BBMODULE_CREATE( blitz2d_gl ){
	return true;
}

BBMODULE_DESTROY( blitz2d_gl ){
	return true;
}
