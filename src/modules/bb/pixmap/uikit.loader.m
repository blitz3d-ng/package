#import <UIKit/UIKit.h>
#include "pixmap.h"

struct BBPixmap *bbLoadPixmapWithUIKit( const char *f ){
  NSString *filename=[NSString stringWithUTF8String:f];
  UIImage* image=[UIImage imageNamed:filename];
  CGImageRef imageRef=[image CGImage];

  struct BBPixmap *pixmap=malloc( sizeof(struct BBPixmap) );
  memset( pixmap,0,sizeof(struct BBPixmap) );

  pixmap->width=CGImageGetWidth( imageRef );
  pixmap->height=CGImageGetHeight( imageRef );

  pixmap->bits=malloc( pixmap->width*pixmap->height*4 );

  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  pixmap->bpp=4;
  CGContextRef context = CGBitmapContextCreate( pixmap->bits,pixmap->width,pixmap->height,8,pixmap->bpp*pixmap->width,colorSpace,kCGImageAlphaPremultipliedLast|kCGBitmapByteOrder32Big );

  CGColorSpaceRelease( colorSpace );
  CGContextDrawImage( context,CGRectMake( 0,0,pixmap->width,pixmap->height ),imageRef );
  CGContextRelease( context );

  return pixmap;
}
