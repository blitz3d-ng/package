#include "../stdutil/stdutil.h"
#include <_types/_uint32_t.h>
#include <bb/graphics/font.h>
#include "canvas.h"
#include "utf8.h"

#include <cmath>
#include <map>
#include <algorithm>

#include "default.glsl.h"

static
GLenum _cube_order[6]={
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left (negative X) face
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // forward (positive Z) face - this is the default.
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right (positive X) face
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // backward (negative Z) face
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // up (positive Y) face
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y  // down (negative Y) face
};

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

GLCanvas::GLCanvas( ContextResources *res,int w,int h,int f ):res(res),pixmap(0),mask(0),width(w),height(h),pixels(0),handle_x(0),handle_y(0),texture(0),framebuffer(0),mode(0),depthbuffer(0),cube_mode(0){
	flags=f;

	setOrigin( 0,0 );
	setHandle( 0,0 );
	setScale( 1.0,1.0 );

	vx=vy=0;
	vw=w;vh=h;

	cube_face=0;
	if( flags&CANVAS_TEX_CUBE ){
		target=GL_TEXTURE_CUBE_MAP;
	}else{
		target=GL_TEXTURE_2D;
	}
}

GLCanvas::GLCanvas( ContextResources *res,int f ):GLCanvas(res,0,0,f){
}


GLCanvas::~GLCanvas(){
	if( pixmap ) {
		delete pixmap;
		pixmap=0;
	}
}

void GLCanvas::resize( int w,int h,float d ){
	width=w;height=h;
}

void GLCanvas::setFont( BBFont *f ){
	font=reinterpret_cast<BBImageFont*>(f);
}

void GLCanvas::setMask( unsigned argb ){
	mask=argb;
	dirty=true;
}

void GLCanvas::setColor( unsigned argb ){
	color[0]=((argb>>16)&255)/255.0f;
	color[1]=((argb>>8)&255)/255.0f;
	color[2]=(argb&255)/255.0f;
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
	vx=x;vy=y;vw=w;vh=h;
	GL( glViewport( x,height-y-h,w,h ) );
	GL( glScissor( x,height-y-h,w,h ) );
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
	// here to support FrontBuffer...
	if( mode==GL_FRONT ){
		GL( glFlush() );
	}
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
	if( !font || t.size()==0 ) return;

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

		int size=font->atlas->width*font->atlas->height;
		std::vector<char> bmp( size*4 );
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

	std::vector<Vertex> verts;

	y+=font->baseline*font->density;

	float awidth=font->atlas->width,aheight=font->atlas->height;

	const char *c=t.c_str();
	utf8_int32_t prev=0,curr=0;
	while( *c ){
		prev=curr;
		c=utf8codepoint( c,&curr );

		BBImageFont::Char c=font->getChar( curr );

		float cx=x+c.bearing_x*font->density,cy=y-c.bearing_y*font->density;

		if( prev ){
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
	uint32_t cfb;
	GL( glGetIntegerv( GL_FRAMEBUFFER_BINDING,(GLint*)&cfb ) );

	float sx,sy,dx,dy;
	s->getScale( &sx,&sy );
	getScale( &dx,&dy );

	// LOGD( "src: scale(%f, %f), %i,%i",sx,sy,s->getWidth(),s->getHeight() );
	// LOGD( "dst: scale(%f, %f), %i,%i",dx,dy,getWidth(),getHeight() );

	GLCanvas *src=(GLCanvas*)s;
	unsigned int rfb=src->framebufferId(),dfb=framebufferId();

	int srcX0=src_x*sx;
	int srcY0=src->getHeight()-(src_h+src_y)*sy;
	int srcX1=src_x*sx+src_w*sx;
	int srcY1=src->getHeight()-src_y*sy;
	int dstX0=x*dx;
	int dstY0=getHeight()-(src_h+y)*dy;
	int dstX1=(x+src_w)*dx;
	int dstY1=getHeight()-y*dy;

	GL( glBindFramebuffer( GL_READ_FRAMEBUFFER,rfb ) );
	GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER,dfb ) );

	GL( glBlitFramebuffer( srcX0,srcY0,srcX1,srcY1,dstX0,dstY0,dstX1,dstY1,GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST ) );

	if( !(flags&CANVAS_TEX_VIDMEM) ){
		downloadData();
	}

	GL( glBindFramebuffer( GL_FRAMEBUFFER,cfb ) );
	flush();

	dirty=true;
}

void GLCanvas::image( BBCanvas *c,int x,int y,bool solid ){
	GLCanvas *src=(GLCanvas*)c;

	GL( glActiveTexture( GL_TEXTURE0 ) );
	src->bind();

	GL( glEnable( GL_BLEND ) );
	GL( glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA ) );

	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE ) );
	GL( glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE ) );

	float white[3]={ 1.0,1.0,1.0 };
	quad( x-src->handle_x,y-src->handle_y,src->getWidth(),src->getHeight(),true,true,1.0,1.0,white );

	GL( glDisable( GL_BLEND ) );
}

bool GLCanvas::collide( int x,int y,const BBCanvas *src,int src_x,int src_y,bool solid )const{
	RTEX( "GLCanvas::collide not implemented" );
	return false;
}

bool GLCanvas::rect_collide( int x,int y,int rect_x,int rect_y,int rect_w,int rect_h,bool solid )const{
	RTEX( "GLCanvas::rect_collide not implemented" );
	return false;
}

bool GLCanvas::lock(){
	if( pixels ) return false;

	pixels=new unsigned char[width*height*4];
	downloadData();
	// RTEX( "GLCanvas::lock not implemented" );
	return true;
}

void GLCanvas::setPixel( int x,int y,unsigned argb ){
	// RTEX( "GLCanvas::setPixel not implemented" );
	lock();
	setPixelFast( x,y,argb );
	unlock();
}

#define UC(c) static_cast<unsigned char>(c)

void GLCanvas::setPixelFast( int x,int y,unsigned argb ){
	// RTEX( "GLCanvas::setPixelFast not implemented" );
}

void GLCanvas::copyPixel( int x,int y,BBCanvas *src,int src_x,int src_y ){
	RTEX( "GLCanvas::copyPixel not implemented" );
}

void GLCanvas::copyPixelFast( int x,int y,BBCanvas *src,int src_x,int src_y ){
	RTEX( "GLCanvas::copyPixelFast not implemented" );
}

unsigned GLCanvas::getPixel( int x,int y )const{
	// RTEX( "GLCanvas::getPixel not implemented" );
	return 0;
}

unsigned GLCanvas::getPixelFast( int x,int y )const{
	// RTEX( "GLCanvas::getPixelFast not implemented" );
	return 0;
}

void GLCanvas::unlock(){
	if( !pixels ) return;

	// RTEX( "GLCanvas::unlock not implemented" );
	uploadData();
	delete[] pixels;pixels=0;
}

void GLCanvas::setCubeMode( int mode ){
	RTEX( "GLCanvas::setCubeMode not implemented" );
}

void GLCanvas::setCubeFace( int face ){
	cube_face=face;
	mode=GL_COLOR_ATTACHMENT0+face;
}

int GLCanvas::getWidth()const{
	return width;
}

int GLCanvas::getHeight()const{
	return height;
}

int GLCanvas::getDepth()const{
	return 8;
}

int GLCanvas::cubeMode()const{
	return cube_mode;
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
	*x=vx;*y=vy;*w=vw;*h=vh;
}

unsigned GLCanvas::getMask()const{
	return mask;
}

unsigned GLCanvas::getColor()const{
	RTEX( "GLCanvas::getColor not implemented" );
	return 0;
}

unsigned GLCanvas::getClsColor()const{
	RTEX( "GLCanvas::getClsColor not implemented" );
	return 0;
}

void GLCanvas::set(){
	GL( glBindFramebuffer( GL_FRAMEBUFFER,framebufferId() ) );
	GL( glDrawBuffer( mode ) );
	GL( glReadBuffer( mode ) );
	setViewport( vx,vy,vw,vh );
}

void GLCanvas::unset(){
	flush();

	// if( !pixmap ){
	// 	BBPixmap *pm=d_new BBPixmap();
	// 	pm->trans=true;
	// 	pm->format=PF_RGBA;
	// 	pm->width=width;
	// 	pm->height=height;
	// 	pm->pitch=4;
	// 	pm->bpp=4;
	// 	int size=pm->width*pm->bpp*pm->height;
	// 	pm->bits=new unsigned char[size];
	// 	pixmap=pm;
	// }

	// GLint cfb;
	// GL( glGetIntegerv( GL_FRAMEBUFFER_BINDING,&cfb ) );
	// GL( glBindFramebuffer( GL_FRAMEBUFFER,framebufferId() ) );
	// GL( glReadPixels( 0,0,pixmap->width,pixmap->height,GL_BGRA,GL_UNSIGNED_BYTE,pixmap->bits  ) );
	// GL( glBindFramebuffer( GL_FRAMEBUFFER,cfb ) );
	// dirty=true;

	// GL( glBindTexture( target,texture ) );
	// GL( glGenerateMipmap( target ) );
	// GL( glBindTexture( target,0 ) );

	GL( glBindFramebuffer( GL_FRAMEBUFFER,0 ) );
}

void GLCanvas::uploadData(){
	if( texture && target!=GL_TEXTURE_2D ) return;

	BBPixmap *pm=0;
	void *data=0;

	// TODO: not super happy with this...
	if( pixels ){
		data=pixels;
	} else if( pixmap ){
		pm=d_new BBPixmap;
		memcpy( pm,pixmap,sizeof(BBPixmap) );

		int size=pm->width*pm->bpp*pm->height;
		pm->bits=new unsigned char[size];
		memcpy( pm->bits,pixmap->bits,size );

		if( flags&CANVAS_TEX_MASK ){
			pm->mask( (mask>>16)&255,(mask>>8)&255,mask&255 );
		}

		data=pm->bits;
	}else{
		// if( target!=GL_TEXTURE_2D ){
		// 	pm=d_new BBPixmap();
		// 	pm->trans=true;
		// 	pm->format=PF_RGBA;
		// 	pm->width=width;
		// 	pm->height=height;
		// 	pm->pitch=4;
		// 	pm->bpp=4;
		// 	int size=pm->width*pm->bpp*pm->height;
		// 	pm->bits=new unsigned char[size];
		// 	pm->fill(255,0,0,1.0);

		// 	data=pm->bits;
		// }
	}

	if( !texture ) GL( glGenTextures( 1,&texture ) );

	GL( glActiveTexture( GL_TEXTURE0 ) );
	GL( glBindTexture( target,texture ) );
	for( int i=0;i<(target==GL_TEXTURE_2D?1:6);i++ ){
		GL( glTexImage2D( target==GL_TEXTURE_2D?target:_cube_order[i],0,GL_RGBA,width,height,0,GL_BGRA,GL_UNSIGNED_BYTE,data ) );
	}
	GL( glGenerateMipmap( target ) );

	delete pm;

	dirty=false;
}

void GLCanvas::downloadData(){
	// BBPixmap *pm=d_new BBPixmap();
	// pm->trans=true;
	// pm->format=PF_RGBA;
	// pm->width=width;
	// pm->height=height;
	// pm->pitch=4;
	// pm->bpp=4;
	// int size=pm->width*pm->bpp*pm->height;
	// pm->bits=new unsigned char[size];
	// pixmap=pm;

	void *bits=pixmap?pixmap->bits:pixels;
	if( bits ){
		GL( glBindFramebuffer( GL_FRAMEBUFFER,framebufferId() ) );
		GL( glReadPixels( 0,0,width,height,GL_BGRA,GL_UNSIGNED_BYTE,bits  ) );
	}
}

unsigned int GLCanvas::textureId(){
	if( texture && !dirty ) return texture;

	uploadData();
	return texture;
}

unsigned int GLCanvas::framebufferId(){
	if( framebuffer || mode==GL_FRONT || mode==GL_BACK ) return framebuffer;

	mode=GL_COLOR_ATTACHMENT0;

	GL( glGenRenderbuffers( 1,&depthbuffer ) );
	GL( glBindRenderbuffer( GL_RENDERBUFFER,depthbuffer ) );
	GL( glRenderbufferStorage( GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,width,height ) );
	GL( glBindRenderbuffer( GL_RENDERBUFFER,0 ) );

	GL( glGenFramebuffers( 1,&framebuffer ) );
	GL( glBindFramebuffer( GL_FRAMEBUFFER,framebuffer ) );
	for( int i=0;i<(target==GL_TEXTURE_2D?1:6);i++ ){
		GL( glFramebufferTexture2D( GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,target==GL_TEXTURE_2D?target:_cube_order[i],textureId(),0 ) );
	}
	GL( glFramebufferRenderbuffer( GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthbuffer ) );

	GLenum status=GL( glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
	if( status!=GL_FRAMEBUFFER_COMPLETE ){
		LOGD( "fb error: %s\n",bbGLFramebufferStatusString( status ) );
	}

	return framebuffer;
}

void GLCanvas::setPixmap( BBPixmap *pm ){
	if( pixmap==pm ) return;

	dirty=true;

	if( !pm ){
		if( pixmap ){
			delete pixmap->bits;
			delete pixmap;
		}
		pixmap=0;
		return;
	}

	pixmap=pm;

	width=pm->width;
	height=pm->height;
	uploadData();

	GL( glBindTexture( target,0 ) );
}

void GLCanvas::setFramebuffer( unsigned int fb,int m ){
	framebuffer=fb;
	mode=m;
}

void GLCanvas::bind(){
	GL( glBindTexture( target,textureId() ) );
}
