
#include "../stdutil/stdutil.h"
#include <bb/blitz2d/font.h>
#include "blitz2d.gles2.h"
#include <cmath>

#include "simple.vert.h"
#include "simple.frag.h"

map<BBImageFont*,unsigned int> font_textures;

void GLES2B2DCanvas::setFont( BBFont *f ){
	font=reinterpret_cast<BBImageFont*>(f);
}

void GLES2B2DCanvas::setMask( unsigned argb ){
}

void GLES2B2DCanvas::setColor( unsigned argb ){
	color[0]=((argb>>16)&255)/255.0f;
	color[1]=((argb>>8)&255)/255.0f;;
	color[2]=(argb&255)/255.0f;;

	// glColor3fv( color );
}

void GLES2B2DCanvas::setClsColor( unsigned argb ){
	int r = (argb >> 16) & 255;
	int g = (argb >> 8) & 255;
	int b = argb & 255;

	// glClearColor( r/255.0f,g/255.0f,b/255.0f,1.0f );
}

void GLES2B2DCanvas::setOrigin( int x,int y ){
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();

	// glTranslatef( x,y,0 );
}

void GLES2B2DCanvas::setHandle( int x,int y ){
	handle_x=x;
	handle_y=y;
}

void GLES2B2DCanvas::setViewport( int x,int y,int w,int h ){
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// glOrtho( 0.0f,w,h,0.0f,-1.0f,1.0f );

	// glViewport( 0,0,w,h );
}

void GLES2B2DCanvas::cls(){
	// glClear(GL_COLOR_BUFFER_BIT);
}

void GLES2B2DCanvas::plot( int x,int y ){
	// glBegin( GL_POINTS );
	// 	glVertex2f( x,y );
	// glEnd();
	// glFlush();
}

void GLES2B2DCanvas::line( int x,int y,int x2,int y2 ){
	// glBegin( GL_LINES );
	// 	glVertex2f( x,y );
	// 	glVertex2f( x2,y2 );
	// glEnd();
	// glFlush();
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

void GLES2B2DCanvas::rect( int x,int y,int w,int h,bool solid ){
	static GLuint program = 0;
	static GLuint vertexBuffer = 0;

	if( program == 0 ){
		GLuint vertShader = compileShader(GL_VERTEX_SHADER, "simple.vert", SIMPLE_VERT, SIMPLE_VERT_SIZE);
		GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, "simple.frag", SIMPLE_FRAG, SIMPLE_FRAG_SIZE);
		// if (vertShader == 0 || fragShader == 0) {
		// 		glfmSetMainLoopFunc(display, NULL);
		// 		return;
		// }
		program = glCreateProgram();

		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);

		glBindAttribLocation(program, 0, "a_position");

		glLinkProgram(program);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}
	glUseProgram(program);
	if( vertexBuffer==0 ){
		glGenBuffers( 1,&vertexBuffer );
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	const size_t stride=sizeof(GLfloat)*3;
	glEnableVertexAttribArray( 0) ;
	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,stride,(void *)0 );
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(GLfloat) * 3));

	GLint posAndSizeLocation=glGetUniformLocation( program,"u_xywh" );
	glUniform4f( posAndSizeLocation,(float)x,(float)y,(float)w,(float)h );

	GLint colorLocation=glGetUniformLocation( program,"u_color" );
	glUniform3fv( colorLocation,1,color );

	const GLfloat vertices[] = {
			// x,y,z, r,g,b
			// app->offsetX + 0.0f, app->offsetY + 1.0f, 0.0,  1.0, 0.0, 0.0,
			// app->offsetX - 0.5f, app->offsetY - 0.5f, 0.0,  0.0, 1.0, 0.0,
			// app->offsetX + 0.5f, app->offsetY - 0.5f, 0.0,  0.0, 0.0, 1.0,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 1.0, 0.0
	};

	glBufferData( GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW );
	glDrawArrays( GL_TRIANGLE_STRIP,0,4 );

	// glPolygonMode( GL_FRONT_AND_BACK,solid?GL_FILL:GL_LINE );
	// glBegin( GL_QUADS );
	// 	glTexCoord2f( 0.0f,0.0f );
	// 	glVertex2f( x,y+h );
	// 	glTexCoord2f( 1.0f,0.0f );
	// 	glVertex2f( x+w,y+h );
	// 	glTexCoord2f( 1.0f,1.0f );
	// 	glVertex2f( x+w,y );
	// 	glTexCoord2f( 0.0f,1.0f );
	// 	glVertex2f( x,y );
	// glEnd();
}

void GLES2B2DCanvas::oval( int x,int y,int w,int h,bool solid ){
	int rx=w/2.0f,ry=h/2.0f;
	int segs=abs(rx)+abs(ry);

	float theta = 2*3.1415926/float(segs);
	float c=cosf(theta),s=sinf(theta),t;

	x+=rx;y+=ry;

	float vx=1.0f,vy=0.0f;

	// glBegin( solid?GL_TRIANGLE_FAN:GL_LINE_LOOP );
	// for( int i=0;i<segs;i++ ){
	// 	glVertex2f( vx*rx+x,vy*ry+y );
	//
	// 	t=vx;
	// 	vx=c*vx-s*vy;
	// 	vy=s*t+c*vy;
	// }
	// glEnd();
	// glFlush();
}

void GLES2B2DCanvas::text( int x,int y,const std::string &t ){
	if( !font ) return;

	// glEnable( GL_ALPHA_TEST );
	// glEnable( GL_BLEND );
	// glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

	unsigned int texture;
	if( font_textures.count(font)==0 ){
		// glGenTextures( 1,&texture );
		font_textures[font]=texture;
	}else{
		texture=font_textures[font];
	}

	if( font->loadChars( t ) ){
		font->rebuildAtlas();

		// glPixelStorei( GL_UNPACK_ALIGNMENT,1 );

		// glBindTexture( GL_TEXTURE_2D,texture );
		// glTexImage2D( GL_TEXTURE_2D,0,GL_ALPHA,font->atlas->width,font->atlas->height,0,GL_ALPHA,GL_UNSIGNED_BYTE,font->atlas->bits );
		// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
		// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE );
		// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
		// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	}

	// glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );

	// glEnable( GL_TEXTURE_2D );
	// glBindTexture( GL_TEXTURE_2D,texture );

	// glBegin( GL_QUADS );
	//
	// y+=font->baseline;
	//
	// for( int i=0;i<t.length();i++ ){
	// 	BBImageFont::Char c=font->getChar( t[i] );
	//
	// 	int cx=x+c.bearing_x,cy=y-c.bearing_y;
	//
	// 	if( i>0 ){
	// 		cx+=font->getKerning( t[i-1],t[i] );
	// 	}
	//
	// 	float l=c.x/(float)font->atlas->width;
	// 	float r=(c.x+c.width)/(float)font->atlas->width;
	// 	float t=c.y/(float)font->atlas->height;
	// 	float b=(c.y+c.height)/(float)font->atlas->height;
	//
	// 	glTexCoord2f( l,b );
	// 	glVertex2f( cx,cy+c.height );
	//
	// 	glTexCoord2f( r,b );
	// 	glVertex2f( cx+c.width,cy+c.height );
	//
	// 	glTexCoord2f( r,t );
	// 	glVertex2f( cx+c.width,cy );
	//
	// 	glTexCoord2f( l,t );
	// 	glVertex2f( cx,cy );
	//
	// 	x+=c.advance;
	// }
	//
	// glEnd();
	// glFlush();

	// glDisable( GL_TEXTURE_2D );
	// glDisable( GL_ALPHA_TEST );
	// glDisable( GL_BLEND );
}

void GLES2B2DCanvas::blit( int x,int y,BBCanvas *s,int src_x,int src_y,int src_w,int src_h,bool solid ){
	unsigned int cfb;
	// glGetIntegerv( GL_FRAMEBUFFER_BINDING,(GLint*)&cfb );

	GLES2B2DCanvas *src=(GLES2B2DCanvas*)s;

	unsigned int rfb=src->framebufferId(),dfb=framebufferId();

	// glBindFramebuffer( GL_READ_FRAMEBUFFER,rfb );
	// glBindFramebuffer( GL_DRAW_FRAMEBUFFER,dfb );
	// glBlitFramebuffer( src_x,src_y,src_w,src_h,x,y,src_w,src_h,GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST );

	// glBindFramebuffer( GL_FRAMEBUFFER,cfb );
}

void GLES2B2DCanvas::image( BBCanvas *c,int x,int y,bool solid ){
	GLES2B2DCanvas *src=(GLES2B2DCanvas*)c;

	src->bind();

	// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
	// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
	// glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );

	const float white[3]={ 1.0f,1.0f,1.0f };
	// glColor3fv( white );
	// rect( x-src->handle_x,y-src->handle_y,src->getWidth(),src->getHeight(),true );
	// glColor3fv( color );

	// glDisable( GL_TEXTURE_2D );
}

void GLES2B2DCanvas::setPixel( int x,int y,unsigned argb ){
	setPixelFast( x,y,argb );
}

#define UC(c) static_cast<unsigned char>(c)

void GLES2B2DCanvas::setPixelFast( int x,int y,unsigned argb ){
	unsigned char rgba[4]={ UC((argb>>16)&255),UC((argb>>8)&255),UC(argb&255),UC((argb>>24)&255) };

	set();
	// glRasterPos2f( x,y+1 );
	// glDrawPixels( 1,1,GL_RGBA,GL_UNSIGNED_BYTE,rgba );
}

BBMODULE_CREATE( blitz2d_gles2 ){
	return true;
}

BBMODULE_DESTROY( blitz2d_gles2 ){
	return true;
}
