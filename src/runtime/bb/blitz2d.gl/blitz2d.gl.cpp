
#include "../../stdutil/stdutil.h"
#include <bb/blitz2d/font.h>
#include "blitz2d.gl.h"
#include <cmath>

map<BBImageFont*,unsigned int> font_textures;

void GLB2DCanvas::cls(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLB2DCanvas::plot( int x,int y ){
	glBegin( GL_POINTS );
		glVertex2f( x,y );
	glEnd();
	glFlush();
}

void GLB2DCanvas::line( int x,int y,int x2,int y2 ){
	glBegin( GL_LINES );
		glVertex2f( x,y );
		glVertex2f( x2,y2 );
	glEnd();
	glFlush();
}

void GLB2DCanvas::rect( int x,int y,int w,int h,bool solid ){
	glPolygonMode( GL_FRONT_AND_BACK,solid?GL_FILL:GL_LINE );
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0f,1.0f );
		glVertex2f( x,y+h );
		glTexCoord2f( 1.0f,1.0f );
		glVertex2f( x+w,y+h );
		glTexCoord2f( 1.0f,0.0f );
		glVertex2f( x+w,y );
		glTexCoord2f( 0.0f,0.0f );
		glVertex2f( x,y );
	glEnd();
	glFlush();
}

void GLB2DCanvas::oval( int x,int y,int w,int h,bool solid ){
	int rx=w/2.0f,ry=h/2.0f;
	int segs=abs(rx)+abs(ry);

	float theta = 2*3.1415926/float(segs);
	float c=cosf(theta),s=sinf(theta),t;

	x+=rx;y+=ry;

	float vx=1.0f,vy=0.0f;

	glBegin( solid?GL_TRIANGLE_FAN:GL_LINE_LOOP );
	for( int i=0;i<segs;i++ ){
		glVertex2f( vx*rx+x,vy*ry+y );

		t=vx;
		vx=c*vx-s*vy;
		vy=s*t+c*vy;
	}
	glEnd();
	glFlush();
}

void GLB2DCanvas::text( int x,int y,const std::string &t ){
	if( !font ) return;

	glEnable( GL_ALPHA_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

	unsigned int texture;
	if( font_textures.count(font)==0 ){
		glGenTextures( 1,&texture );
		font_textures[font]=texture;
	}else{
		texture=font_textures[font];
	}

	if( font->loadChars( t ) ){
		font->rebuildAtlas();

		glPixelStorei( GL_UNPACK_ALIGNMENT,1 );

		glBindTexture( GL_TEXTURE_2D,texture );
		glTexImage2D( GL_TEXTURE_2D,0,GL_ALPHA,font->atlas->width,font->atlas->height,0,GL_ALPHA,GL_UNSIGNED_BYTE,font->atlas->bits );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	}

	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D,texture );

	glBegin( GL_QUADS );

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

		glTexCoord2f( l,b );
		glVertex2f( cx,cy+c.height );

		glTexCoord2f( r,b );
		glVertex2f( cx+c.width,cy+c.height );

		glTexCoord2f( r,t );
		glVertex2f( cx+c.width,cy );

		glTexCoord2f( l,t );
		glVertex2f( cx,cy );

		x+=c.advance;
	}

	glEnd();
	glFlush();
}

void GLB2DCanvas::image( BBCanvas *c,int x,int y ){

}

BBMODULE_CREATE( blitz2d_gl ){
#if defined(__linux__) || defined(_WIN32)
	return glewInit()==GLEW_OK;
#else
	return true;
#endif
}

BBMODULE_DESTROY( blitz2d_gl ){
	return true;
}
