#ifndef BB_GRAPHICS_COMMANDS_H
#define BB_GRAPHICS_COMMANDS_H

#include <bb/blitz/module.h>
#include <bb/graphics/graphics.h>

#ifdef __cplusplus
extern "C" {
#endif

// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.

// renderer
bb_int_t BBCALL bbCountRenderers(  );
BBStr * BBCALL bbRendererName( bb_int_t renderer );
bb_int_t BBCALL bbSetRenderer( BBStr *name );

//gfx driver info
bb_int_t BBCALL bbCountGfxDrivers(  );
BBStr * BBCALL bbGfxDriverName( bb_int_t driver );
void BBCALL bbSetGfxDriver( bb_int_t driver );

//gfx mode info
bb_int_t BBCALL bbCountGfxModes(  );
bb_int_t BBCALL bbGfxModeExists( bb_int_t width,bb_int_t height,bb_int_t depth );
bb_int_t BBCALL bbGfxModeWidth( bb_int_t mode );
bb_int_t BBCALL bbGfxModeHeight( bb_int_t mode );
bb_int_t BBCALL bbGfxModeDepth( bb_int_t mode );
bb_int_t BBCALL bbAvailVidMem(  );
bb_int_t BBCALL bbTotalVidMem(  );
bb_int_t BBCALL bbGfxDriver3D( bb_int_t driver );
bb_int_t BBCALL bbCountGfxModes3D(  );
bb_int_t BBCALL bbGfxMode3DExists( bb_int_t width,bb_int_t height,bb_int_t depth );
bb_int_t BBCALL bbGfxMode3D( bb_int_t mode );
bb_int_t BBCALL bbWindowed3D(  );

//display mode
void BBCALL bbGraphics( bb_int_t width,bb_int_t height,bb_int_t depth,bb_int_t mode );
void BBCALL bbGraphics3D( bb_int_t width,bb_int_t height,bb_int_t depth,bb_int_t mode );
void BBCALL bbEndGraphics(  );
bb_int_t BBCALL bbGraphicsLost(  );
bb_int_t BBCALL bbGraphicsOpen(  );
void BBCALL bbSetGamma( bb_int_t src_red,bb_int_t src_green,bb_int_t src_blue,bb_float_t dest_red,bb_float_t dest_green,bb_float_t dest_blue );
void BBCALL bbUpdateGamma( bb_int_t calibrate );
bb_float_t BBCALL bbGammaRed( bb_int_t red );
bb_float_t BBCALL bbGammaGreen( bb_int_t green );
bb_float_t BBCALL bbGammaBlue( bb_int_t blue );
BBCanvas * BBCALL bbFrontBuffer(  );
BBCanvas * BBCALL bbBackBuffer(  );
bb_int_t BBCALL bbScanLine(  );
void BBCALL bbVWait( bb_int_t frames );
void BBCALL bbFlip( bb_int_t vwait );
bb_int_t BBCALL bbGraphicsWidth(  );
bb_int_t BBCALL bbGraphicsHeight(  );
bb_int_t BBCALL bbGraphicsDepth(  );

//buffer management
void BBCALL bbSetBuffer( BBCanvas *buffer );
BBCanvas * BBCALL bbGraphicsBuffer(  );
bb_int_t BBCALL bbLoadBuffer( BBCanvas *buffer,BBStr *bmpfile );
bb_int_t BBCALL bbSaveBuffer( BBCanvas *buffer,BBStr *bmpfile );
void BBCALL bbBufferDirty( BBCanvas *buffer );

//fast pixel reads/write
void BBCALL bbLockBuffer( BBCanvas *buffer );
void BBCALL bbUnlockBuffer( BBCanvas *buffer );
bb_int_t BBCALL bbReadPixel( bb_int_t x,bb_int_t y,BBCanvas *buffer );
void BBCALL bbWritePixel( bb_int_t x,bb_int_t y,bb_int_t argb,BBCanvas *buffer );
bb_int_t BBCALL bbReadPixelFast( bb_int_t x,bb_int_t y,BBCanvas *buffer );
void BBCALL bbWritePixelFast( bb_int_t x,bb_int_t y,bb_int_t argb,BBCanvas *buffer );
void BBCALL bbCopyPixel( bb_int_t src_x,bb_int_t src_y,BBCanvas *src_buffer,bb_int_t dest_x,bb_int_t dest_y,BBCanvas *dest_buffer );
void BBCALL bbCopyPixelFast( bb_int_t src_x,bb_int_t src_y,BBCanvas *src_buffer,bb_int_t dest_x,bb_int_t dest_y,BBCanvas *dest_buffer );
void BBCALL bbCopyRect( bb_int_t source_x,bb_int_t source_y,bb_int_t width,bb_int_t height,bb_int_t dest_x,bb_int_t dest_y,BBCanvas *src_buffer,BBCanvas *dest_buffer );

//rendering
void BBCALL bbOrigin( bb_int_t x,bb_int_t y );
void BBCALL bbViewport( bb_int_t x,bb_int_t y,bb_int_t width,bb_int_t height );
void BBCALL bbColor( bb_int_t red,bb_int_t green,bb_int_t blue );
void BBCALL bbGetColor( bb_int_t x,bb_int_t y );
bb_int_t BBCALL bbColorRed(  );
bb_int_t BBCALL bbColorGreen(  );
bb_int_t BBCALL bbColorBlue(  );
void BBCALL bbClsColor( bb_int_t red,bb_int_t green,bb_int_t blue );
void BBCALL bbCls(  );
void BBCALL bbPlot( bb_int_t x,bb_int_t y );
void BBCALL bbRect( bb_int_t x,bb_int_t y,bb_int_t width,bb_int_t height,bb_int_t solid );
void BBCALL bbOval( bb_int_t x,bb_int_t y,bb_int_t width,bb_int_t height,bb_int_t solid );
void BBCALL bbLine( bb_int_t x1,bb_int_t y1,bb_int_t x2,bb_int_t y2 );
void BBCALL bbText( bb_int_t x,bb_int_t y,BBStr *text,bb_int_t centre_x,bb_int_t centre_y );

//fonts
BBFont * BBCALL bbLoadFont( BBStr *fontname,bb_int_t height,bb_int_t bold,bb_int_t italic,bb_int_t underline );
void BBCALL bbFreeFont( BBFont *font );
void BBCALL bbSetFont( BBFont *font );
bb_int_t BBCALL bbFontWidth(  );
bb_int_t BBCALL bbFontHeight(  );
bb_int_t BBCALL bbStringWidth( BBStr *string );
bb_int_t BBCALL bbStringHeight( BBStr *string );

//movies
BBMovie * BBCALL bbOpenMovie( BBStr *file );
bb_int_t BBCALL bbDrawMovie( BBMovie *movie,bb_int_t x,bb_int_t y,bb_int_t w,bb_int_t h );
bb_int_t BBCALL bbMovieWidth( BBMovie *movie );
bb_int_t BBCALL bbMovieHeight( BBMovie *movie );
bb_int_t BBCALL bbMoviePlaying( BBMovie *movie );
void BBCALL bbCloseMovie( BBMovie *movie );

//images
BBImage * BBCALL bbLoadImage( BBStr *bmpfile );
BBImage * BBCALL bbLoadAnimImage( BBStr *bmpfile,bb_int_t cellwidth,bb_int_t cellheight,bb_int_t first,bb_int_t count );
BBImage * BBCALL bbCopyImage( BBImage *image );
BBImage * BBCALL bbCreateImage( bb_int_t width,bb_int_t height,bb_int_t frames );
void BBCALL bbFreeImage( BBImage *image );
bb_int_t BBCALL bbSaveImage( BBImage *image,BBStr *bmpfile,bb_int_t frame );
void BBCALL bbGrabImage( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t frame );
BBCanvas * BBCALL bbImageBuffer( BBImage *image,bb_int_t frame );
void BBCALL bbDrawImage( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t frame );
void BBCALL bbDrawBlock( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t frame );
void BBCALL bbTileImage( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t frame );
void BBCALL bbTileBlock( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t frame );
void BBCALL bbDrawImageRect( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t rect_x,bb_int_t rect_y,bb_int_t rect_width,bb_int_t rect_height,bb_int_t frame );
void BBCALL bbDrawBlockRect( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t rect_x,bb_int_t rect_y,bb_int_t rect_width,bb_int_t rect_height,bb_int_t frame );
void BBCALL bbMaskImage( BBImage *image,bb_int_t red,bb_int_t green,bb_int_t blue );
void BBCALL bbHandleImage( BBImage *image,bb_int_t x,bb_int_t y );
void BBCALL bbMidHandle( BBImage *image );
void BBCALL bbAutoMidHandle( bb_int_t enable );
bb_int_t BBCALL bbImageWidth( BBImage *image );
bb_int_t BBCALL bbImageHeight( BBImage *image );
bb_int_t BBCALL bbImageXHandle( BBImage *image );
bb_int_t BBCALL bbImageYHandle( BBImage *image );
void BBCALL bbScaleImage( BBImage *image,bb_float_t xscale,bb_float_t yscale );
void BBCALL bbResizeImage( BBImage *image,bb_float_t width,bb_float_t height );
void BBCALL bbRotateImage( BBImage *image,bb_float_t angle );
void BBCALL bbTFormImage( BBImage *image,bb_float_t a,bb_float_t b,bb_float_t c,bb_float_t d );
void BBCALL bbTFormFilter( bb_int_t enable );
bb_int_t BBCALL bbImagesOverlap( BBImage *image1,bb_int_t x1,bb_int_t y1,BBImage *image2,bb_int_t x2,bb_int_t y2 );
bb_int_t BBCALL bbImagesCollide( BBImage *image1,bb_int_t x1,bb_int_t y1,bb_int_t frame1,BBImage *image2,bb_int_t x2,bb_int_t y2,bb_int_t frame2 );
bb_int_t BBCALL bbRectsOverlap( bb_int_t x1,bb_int_t y1,bb_int_t width1,bb_int_t height1,bb_int_t x2,bb_int_t y2,bb_int_t width2,bb_int_t height2 );
bb_int_t BBCALL bbImageRectOverlap( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t rect_x,bb_int_t rect_y,bb_int_t rect_width,bb_int_t rect_height );
bb_int_t BBCALL bbImageRectCollide( BBImage *image,bb_int_t x,bb_int_t y,bb_int_t frame,bb_int_t rect_x,bb_int_t rect_y,bb_int_t rect_width,bb_int_t rect_height );

//console emu
void BBCALL bbWrite( BBStr *string );
void BBCALL bbPrint( BBStr *string );
BBStr * BBCALL bbInput( BBStr *prompt );
void BBCALL bbLocate( bb_int_t x,bb_int_t y );

#ifdef __cplusplus
}
#endif


#endif
