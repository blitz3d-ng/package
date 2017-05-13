
#include "../../stdutil/stdutil.h"
#include <bb/blitz2d/font.h>
#include "blitz2d.gl.h"

map<BBImageFont*,unsigned int> font_textures;

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

		float tl[2]={ c.x/(float)font->atlas->width,c.y/(float)font->atlas->height };
		float tr[2]={ (c.x+c.width)/(float)font->atlas->width,c.y/(float)font->atlas->height };
		float bl[2]={ c.x/(float)font->atlas->width,(c.y+c.height)/(float)font->atlas->height };
		float br[2]={ (c.x+c.width)/(float)font->atlas->width,(c.y+c.height)/(float)font->atlas->height };

		float white[3]={ 1.0f,1.0f,1.0f };
		glColor3fv( white );

		glTexCoord2fv( bl );
		glVertex2f( cx,cy+c.height );

		glTexCoord2fv( br );
		glVertex2f( cx+c.width,cy+c.height );

		glTexCoord2fv( tr );
		glVertex2f( cx+c.width,cy );

		glTexCoord2fv( tl );
		glVertex2f( cx,cy );

		x+=c.advance;
	}

	glEnd();
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
