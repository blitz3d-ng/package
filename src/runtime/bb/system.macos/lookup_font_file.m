#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

const char *lookupFontFile( const char *fontName ){
  NSString *fn = [NSString stringWithUTF8String:fontName];

  NSFont *font=[NSFont fontWithName:fn size:10];
  CTFontDescriptorRef fontRef = CTFontDescriptorCreateWithNameAndSize ((CFStringRef)[font fontName], [font pointSize]);
  CFURLRef url = (CFURLRef)CTFontDescriptorCopyAttribute(fontRef, kCTFontURLAttribute);
  NSString *fp = [NSString stringWithString:[(NSURL *)CFBridgingRelease(url) path]];

  return [fp UTF8String];
}
