#include "../stdutil/stdutil.h"
#include <bb/graphics/font.h>
#include "canvas.h"
#include <utf8.h>

#include <cmath>
#include <map>
#include <algorithm>

#include "default.glsl.h"

struct UniformState{
	float xywh[4];
	float res[2];
	float texscale[2];
	float color[3];
	int texenabled;
	float scale[2];
};

struct Vertex{
	float x,y;
	float u,v;
};

static
bool makeProgram( ContextResources *res,float sx,float sy,float tx,float ty,bool tex_enabled,float resx,float resy,float x,float y,float width,float height,float color[3] ){
	if( !glIsProgram( res->default_program ) ){
		// LOGD( "rebuilding 2d shader...\n" );

		std::string src( DEFAULT_GLSL,DEFAULT_GLSL+DEFAULT_GLSL_SIZE );
		res->default_program=GL( _bbGLCompileProgram( "default.glsl",src ) );
		if( !res->default_program ){
			return false;
		}

		GL( glUseProgram( res->default_program ) );

		GLint texLocation=GL( glGetUniformLocation( res->default_program,"u_tex" ) );
		GL( glUniform1i( texLocation,0 ) );

		GLint idx=GL( glGetUniformBlockIndex( res->default_program,"BBRenderState" ) );
		GL( glUniformBlockBinding( res->default_program,idx,0 ) );
	}

	GL( glUseProgram( res->default_program ) );

	UniformState us={ 0 };
	us.texscale[0]=tx;us.texscale[1]=ty;
	us.texenabled=tex_enabled;
	us.res[0]=resx;us.res[1]=resy;
	us.xywh[0]=x;us.xywh[1]=y;us.xywh[2]=width;us.xywh[3]=height;
	us.color[0]=color[0];us.color[1]=color[1];us.color[2]=color[2];
	us.scale[0]=sx;us.scale[1]=sy;

	if( res->ubo ){
		GL( glBindBuffer( GL_UNIFORM_BUFFER,res->ubo ) );
	}else{
		GL( glGenBuffers( 1,&res->ubo ) );
		GL( glBindBuffer( GL_UNIFORM_BUFFER,res->ubo ) );
		GL( glBindBufferRange( GL_UNIFORM_BUFFER,0,res->ubo,0,sizeof(us) ) );
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

// --- GLCanvas ---

GLCanvas::GLCanvas( ContextResources *res,int f ):res(res),width(0),height(0),pixels(0),scale_x(1.0),scale_y(1.0),origin_x(0.0),origin_y(0.0),handle_x(0),handle_y(0){
	flags=f;
}

void GLCanvas::resize( int w,int h,float d ){
	width=w;height=h;
}

void GLCanvas::setFont( BBFont *f ){
	font=reinterpret_cast<BBImageFont*>(f);
}

void GLCanvas::setMask( unsigned argb ){
}

void GLCanvas::setColor( unsigned argb ){
	color[0]=((argb>>16)&255)/255.0f;
	color[1]=((argb>>8)&255)/255.0f;
	color[2]=(argb&255)/255.0f;;
}

void GLCanvas::setClsColor( unsigned argb ){
	int r = (argb >> 16) & 255;
	int g = (argb >> 8) & 255;
	int b = argb & 255;

	GL( glClearColor( r/255.0f,g/255.0f,b/255.0f,1.0f ) );
}

void GLCanvas::setOrigin( int x,int y ){
	origin_x=x;
	origin_y=y;
}

void GLCanvas::setScale( float x,float y ){
	scale_x=x;
	scale_y=y;
}

void GLCanvas::setHandle( int x,int y ){
	handle_x=x;
	handle_y=y;
}

void GLCanvas::setViewport( int x,int y,int w,int h ){
	GL( glEnable( GL_SCISSOR_TEST ) );
	GL( glViewport( x,y,w,h ) );
	GL( glScissor( x,y,w,h ) );
}

void GLCanvas::cls(){
	GL( glClear( GL_COLOR_BUFFER_BIT ) );
}

void GLCanvas::plot( int x,int y ){
	if( res->plot_buffer==0 ){
		const Vertex vertices[1] = {
			{ 0.0,0.0,0.0,0.0 }
		};

		initArrays( 1,&res->plot_buffer,&res->plot_array );
		GL( glBindBuffer( GL_ARRAY_BUFFER,res->plot_buffer ) );
		GL( glBufferData( GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW ) );
	}

#ifdef BB_DESKTOP
	GL( glEnable(GL_PROGRAM_POINT_SIZE) );
#endif

	makeProgram( res,scale_x,scale_y,1.0,1.0,false,width,height,x,y,1.0,1.0,color );

	GL( glBindVertexArray( res->plot_array ) );
	GL( glDrawArrays( GL_POINTS,0,1 ) );
	GL( glBindVertexArray( 0 ) );
	flush();

#ifdef BB_DESKTOP
	GL( glDisable(GL_PROGRAM_POINT_SIZE) );
#endif
}

void GLCanvas::line( int x,int y,int x2,int y2 ){
	const Vertex vertices[2] = {
		{ (float)x,(float)y,0.0,0.0 },
		{ (float)x2,(float)y2,0.0,0.0 }
	};

	initArrays( 1,&res->line_buffer,&res->line_array );

	GL( glBindBuffer( GL_ARRAY_BUFFER,res->line_buffer ) );
	GL( glBufferData( GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW ) );

	makeProgram( res,scale_x,scale_y,1.0,1.0,false,width,height,0.0,0.0,1.0,1.0,color );

	GL( glBindVertexArray( res->line_array ) );
	GL( glDrawArrays( GL_LINES,0,2 ) );
	GL( glBindVertexArray( 0 ) );
	flush();
}

void GLCanvas::rect( int x,int y,int w,int h,bool solid ){
	quad( x,y,w,h,solid,false,1.0,1.0,color );
}

void GLCanvas::flush(){
	if( !needs_flush ) return;

	// here to support FrontBuffer...
	GL( glFlush() );
}

void GLCanvas::quad( int x,int y,int w,int h,bool solid,bool texenabled,float tx,float ty,float color[3] ){
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

	if( res->quad_buffer[0]==0 ){
		initArrays( 2,res->quad_buffer,res->quad_array );

		GL( glBindBuffer( GL_ARRAY_BUFFER,res->quad_buffer[0] ) );
		GL( glBufferData( GL_ARRAY_BUFFER,sizeof(vertices[0]),vertices[0],GL_STATIC_DRAW ) );

		GL( glBindBuffer( GL_ARRAY_BUFFER,res->quad_buffer[1] ) );
		GL( glBufferData( GL_ARRAY_BUFFER,sizeof(vertices[1]),vertices[1],GL_STATIC_DRAW ) );

		GL( glBindBuffer( GL_ARRAY_BUFFER,0 ) );
	}

	makeProgram( res,scale_x,scale_y,tx,ty,texenabled,width,height,x,y,w,h,color );

	int i=solid?0:1;
	GL( glBindVertexArray( res->quad_array[i] ) );
	GL( glDrawArrays( solid?GL_TRIANGLE_STRIP:GL_LINE_LOOP,0,4 ) );
	GL( glBindVertexArray( 0 ) );
	flush();
}

void GLCanvas::oval( int x,int y,int w,int h,bool solid ){
	int rx=w/2.0f,ry=h/2.0f;
	int segs=abs(rx)+abs(ry);

	float theta = 2*3.1415926/float(segs);
	float c=cosf(theta),s=sinf(theta),t;

	x+=rx;y+=ry;

	float vx=1.0f,vy=0.0f;

	std::vector<Vertex> verts;
	for( int i=0;i<segs;i++ ){
		verts.push_back( { vx*rx+x,vy*ry+y,0.0,0.0 } );

		t=vx;
		vx=c*vx-s*vy;
		vy=s*t+c*vy;
	}
	std::reverse( verts.begin(),verts.end() );

	if( res->oval_buffer==0 ){
		initArrays( 1,&res->oval_buffer,&res->oval_array );
	}

	makeProgram( res,scale_x,scale_y,1.0,1.0,false,width,height,0.0,0.0,1.0,1.0,color );

	GL( glBindBuffer( GL_ARRAY_BUFFER,res->oval_buffer ) );
	GL( glBufferData( GL_ARRAY_BUFFER,sizeof(Vertex)*verts.size(),verts.data(),GL_DYNAMIC_DRAW ) );

	GL( glBindVertexArray( res->oval_array ) );
	GL( glDrawArrays( solid?GL_TRIANGLE_FAN:GL_LINE_LOOP,0,verts.size() ) );
	GL( glBindVertexArray( 0 ) );
	flush();
}

void GLCanvas::text( int x,int y,const std::string &t ){
	if( !font ) return;

	unsigned int texture;
	if( res->font_textures.count(font)==0 ){
		GL( glGenTextures( 1,&texture ) );
		res->font_textures[font]=texture;
	}else{
		texture=res->font_textures[font];
	}

	GL( glActiveTexture( GL_TEXTURE0 ) );
	GL( glBindTexture( GL_TEXTURE_2D,texture ) );

	if( font->loadChars( t ) ){
		font->rebuildAtlas();
		if( font->atlas ){
			int size=font->atlas->width*font->atlas->height;
			std::vector<unsigned char> bmp( size*4 );
			for( int i=0;i<font->atlas->width*font->atlas->height;i++ ){
				bmp[i*4+0]=bmp[i*4+1]=bmp[i*4+2]=255;
				bmp[i*4+3]=font->atlas->bits[i];
			}

			GL( glPixelStorei( GL_UNPACK_ALIGNMENT,1 ) );
			GL( glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,font->atlas->width,font->atlas->height,0,GL_RGBA,GL_UNSIGNED_BYTE,bmp.data() ) );
			GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE ) );
			GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE ) );
			GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR ) );
			GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR ) );
		}
	}
	if( !font->atlas ) return;

	std::vector<Vertex> verts;

	y+=font->baseline*font->density;

	float awidth=font->atlas->width,aheight=font->atlas->height;

	const char *c=t.c_str();
	utf8_int32_t curr=0,prev=0;
	while( *c ){
		prev=curr;
		c=utf8codepoint( c,&curr );
		BBImageFont::Char c=font->getChar( curr );

		float cx=x+c.bearing_x*font->density,cy=y-c.bearing_y*font->density;

		if( prev>0 ){
			cx+=font->getKerning( prev,curr );
		}

		float l=c.x/awidth;
		float r=(c.x+c.width)/awidth;
		float t=c.y/aheight;
		float b=(c.y+c.height)/aheight;

		Vertex lb={ cx,cy+c.height*font->density,l,b },
		       rb={ cx+c.width*font->density,cy+c.height*font->density,r,b },
		       rt={ cx+c.width*font->density,cy,r,t },
		       lt={ cx,cy,l,t };

		verts.push_back( lb );verts.push_back( rb );verts.push_back( rt );
		verts.push_back( lb );verts.push_back( rt );verts.push_back( lt );

		x+=c.advance*font->density;
	}

	GL( glEnable( GL_BLEND ) );
	GL( glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA ) );

	if( res->text_buffer==0 ){
		initArrays( 1,&res->text_buffer,&res->text_array );
	}

	makeProgram( res,scale_x,scale_y,1.0,1.0,true,width,height,0.0,0.0,1.0,1.0,color );

	GL( glBindBuffer( GL_ARRAY_BUFFER,res->text_buffer ) );
	GL( glBufferData( GL_ARRAY_BUFFER,sizeof(Vertex)*verts.size(),verts.data(),GL_DYNAMIC_DRAW ) );
	GL( glBindBuffer( GL_ARRAY_BUFFER,0 ) );

	GL( glBindVertexArray( res->text_array ) );
	GL( glDrawArrays( GL_TRIANGLES,0,verts.size() ) );
	GL( glBindVertexArray( 0 ) );

	GL( glDisable( GL_BLEND ) );
	flush();
}

void GLCanvas::blit( int x,int y,BBCanvas *s,int src_x,int src_y,int src_w,int src_h,bool solid ){
	unsigned int cfb;
	GL( glGetIntegerv( GL_FRAMEBUFFER_BINDING,(GLint*)&cfb ) );

	GLCanvas *src=(GLCanvas*)s;

	unsigned int rfb=src->framebufferId(),dfb=framebufferId();

	GL( glBindFramebuffer( GL_READ_FRAMEBUFFER,rfb ) );
	GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER,dfb ) );
	GL( glBlitFramebuffer( src_x,src_y,src_w,src_h,x,src_h,src_w,y,GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST ) );

	GL( glBindFramebuffer( GL_FRAMEBUFFER,cfb ) );
	flush();
}

void GLCanvas::image( BBCanvas *c,int x,int y,bool solid ){
	GLTextureCanvas *src=(GLTextureCanvas*)c;

	src->bind();

	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE ) );


	float white[3]={ 1.0,1.0,1.0 };
	quad( x-src->handle_x,y-src->handle_y,src->getWidth(),src->getHeight(),true,true,1.0,1.0,white );
}

bool GLCanvas::collide( int x,int y,const BBCanvas *src,int src_x,int src_y,bool solid )const{
	return false;
}

bool GLCanvas::rect_collide( int x,int y,int rect_x,int rect_y,int rect_w,int rect_h,bool solid )const{
	return false;
}

bool GLCanvas::lock()const{
	if( pixels ) return false;

	pixels=new unsigned char[width*height*4];

	bind();
#ifdef BB_DESKTOP
	GL( glGetTexImage( GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels ) );
#endif
	return true;
}

void GLCanvas::setPixel( int x,int y,unsigned argb ){
	float prev_color[3]={ color[0],color[1],color[2] };
	color[0]=((argb>>16)&255)/255.0f;
	color[1]=((argb>>8)&255)/255.0f;;
	color[2]=(argb&255)/255.0f;;

	plot( x,y );

	color[0]=prev_color[0];
	color[1]=prev_color[1];
	color[2]=prev_color[2];
}

#define UC(c) static_cast<unsigned char>(c)

void GLCanvas::setPixelFast( int x,int y,unsigned argb ){
	// unsigned char rgba[4]={ UC((argb>>16)&255),UC((argb>>8)&255),UC(argb&255),UC((argb>>24)&255) };
	setPixel( x,y,argb );
}

void GLCanvas::copyPixel( int x,int y,BBCanvas *src,int src_x,int src_y ){
}

void GLCanvas::copyPixelFast( int x,int y,BBCanvas *src,int src_x,int src_y ){
}

unsigned GLCanvas::getPixel( int x,int y )const{
	return 0;
}

unsigned GLCanvas::getPixelFast( int x,int y )const{
	// y=height-y;
	// return pixels[(y*width+x)*4];
	return 0;
}

void GLCanvas::unlock()const{
	delete[] pixels;pixels=0;
}

void GLCanvas::setCubeMode( int mode ){
}

void GLCanvas::setCubeFace( int face ){
}

int GLCanvas::getWidth()const{
	return width;
}

int GLCanvas::getHeight()const{
	return height;
}

int GLCanvas::cubeMode()const{
	return 0;
}

void GLCanvas::getOrigin( int *x,int *y )const{
	*x=origin_x;*y=origin_y;
}

void GLCanvas::getScale( float *x,float *y )const{
	*x=scale_x;*y=scale_y;
}

void GLCanvas::getHandle( int *x,int *y )const{
	*x=handle_x;*y=handle_y;
}

void GLCanvas::getViewport( int *x,int *y,int *w,int *h )const{
	*x=0;*y=0;*w=getWidth();*h=getHeight();
}

unsigned GLCanvas::getMask()const{
	return 0;
}

unsigned GLCanvas::getColor()const{
	return 0;
}

unsigned GLCanvas::getClsColor()const{
	return 0;
}

// --- GLTextureCanvas ---

GLTextureCanvas::GLTextureCanvas( ContextResources *res,int f ):GLCanvas( res,f ),texture(0),framebuffer(0),depthbuffer(0),twidth(0),theight(0){
}

GLTextureCanvas::GLTextureCanvas( ContextResources *res,int w,int h,int f ):GLTextureCanvas(res,f){
	width=w;
	height=h;
}

GLTextureCanvas::GLTextureCanvas( ContextResources *res,BBPixmap *pixmap,int f ):GLTextureCanvas(res,f){
	if( pixmap ) setPixmap( pixmap );
}

int GLTextureCanvas::getDepth()const{
	return 8;
}

void GLTextureCanvas::set(){
	GL( glBindFramebuffer( GL_FRAMEBUFFER,framebufferId() ) );
	GLenum bufs[]={ GL_COLOR_ATTACHMENT0 };
	GL( glDrawBuffers( 1,bufs ) );
}

void GLTextureCanvas::unset(){
	flush();

	GL( glBindTexture( GL_TEXTURE_2D,texture ) );
	GL( glGenerateMipmap( GL_TEXTURE_2D ) );
}

void GLTextureCanvas::uploadData( void *data ){
	if( !texture ) GL( glGenTextures( 1,&texture ) );
	GL( glActiveTexture( GL_TEXTURE0 ) );
	GL( glBindTexture( GL_TEXTURE_2D,texture ) );
	GL( glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_BGRA,GL_UNSIGNED_BYTE,data ) );
	GL( glGenerateMipmap( GL_TEXTURE_2D ) );
}

unsigned int GLTextureCanvas::textureId(){
	if( texture ) return texture;

	uploadData(0);
	return texture;
}

unsigned int GLTextureCanvas::framebufferId(){
	if( framebuffer ) return framebuffer;

	GL( glGenRenderbuffers( 1,&depthbuffer ) );
	GL( glBindRenderbuffer( GL_RENDERBUFFER,depthbuffer ) );
	GL( glRenderbufferStorage( GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,width,height ) );
	GL( glBindRenderbuffer( GL_RENDERBUFFER,0 ) );

	GL( glGenFramebuffers( 1,&framebuffer ) );
	GL( glBindFramebuffer( GL_FRAMEBUFFER,framebuffer ) );
	GL( glFramebufferTexture2D( GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textureId(),0 ) );
	GL( glFramebufferRenderbuffer( GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthbuffer ) );

	GLenum status=GL( glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
	if( status!=GL_FRAMEBUFFER_COMPLETE ){
		LOGD( "fb error: %s\n",bbGLFramebufferStatusString( status ) );
	}

	return framebuffer;
}

void GLTextureCanvas::setPixmap( BBPixmap *pm ){
	if( flags&CANVAS_TEX_ALPHA ){
		if( flags&CANVAS_TEX_MASK ){
			pm->mask( 0,0,0 );
		}else{
			pm->buildAlpha( false );
		}
	}

	width=pm->width;
	height=pm->height;
	uploadData( pm->bits );
	GL( glBindTexture( GL_TEXTURE_2D,0 ) );
}

void GLTextureCanvas::bind()const{
	GL( glBindTexture( GL_TEXTURE_2D,texture ) );
}


// --- GLDefaultCanvas ---

void GLDefaultCanvas::bind()const{
}

GLDefaultCanvas::GLDefaultCanvas( ContextResources *res,unsigned int fb,int m,int f ):GLCanvas(res,f),framebuffer(fb),mode(m){
	needs_flush=(mode==GL_FRONT);
	set();
	GL( glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) );
}

void GLDefaultCanvas::unset(){
	flush();
}

void GLDefaultCanvas::set(){
	GL( glBindFramebuffer( GL_FRAMEBUFFER,framebuffer ) );

#ifdef BB_DESKTOP
	if( framebuffer==0 ){
		GL( glDrawBuffer( mode ) );
	}
#endif

	GL( glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) );
}

unsigned int GLDefaultCanvas::framebufferId(){
	return framebuffer;
}

void GLDefaultCanvas::blit( int x,int y,BBCanvas *src,int src_x,int src_y,int src_w,int src_h,bool solid ){
}

int GLDefaultCanvas::getDepth()const{
	return 8;
}

BBMODULE_CREATE( blitz2d_gl ){
	return true;
}

BBMODULE_DESTROY( blitz2d_gl ){
	return true;
}
