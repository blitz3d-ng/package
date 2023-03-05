#import <UIKit/UIKit.h>
#include "pixmap.h"

struct BBPixmap *bbLoadPixmapWithUIKit( const char *f ){
	NSString *filename=[NSString stringWithUTF8String:f];
	UIImage* image=[UIImage imageNamed:filename];
	if( image==0 ){
		return 0;
	}

	CGImageRef imageRef=[image CGImage];

	struct BBPixmap *pixmap=malloc( sizeof(struct BBPixmap) );
	memset( pixmap,0,sizeof(struct BBPixmap) );

	pixmap->width=CGImageGetWidth( imageRef );
	pixmap->height=CGImageGetHeight( imageRef );
	pixmap->bits=malloc( pixmap->width*pixmap->height*4 );
	pixmap->bpp=4;

	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGContextRef context = CGBitmapContextCreate( pixmap->bits,pixmap->width,pixmap->height,8,pixmap->bpp*pixmap->width,colorSpace,kCGImageAlphaPremultipliedLast|kCGBitmapByteOrder32Big );

	// CGContextTranslateCTM( context,0,pixmap->height );
	// CGContextScaleCTM( context,1.0,-1.0 );

	CGContextDrawImage( context,CGRectMake( 0,0,pixmap->width,pixmap->height ),imageRef );

	CGColorSpaceRelease( colorSpace );
	CGContextRelease( context );

	return pixmap;
}
