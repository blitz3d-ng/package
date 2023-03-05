// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.

#include <bb/blitz/module.h>
#include <bb/graphics/graphics.h>

BBMODULE_LINK( graphics ){
	rtSym( "%CountGfxDrivers",bbCountGfxDrivers );
	rtSym( "$GfxDriverName%driver",bbGfxDriverName );
	rtSym( "SetGfxDriver%driver",bbSetGfxDriver );
	rtSym( "%CountGfxModes",bbCountGfxModes );
	rtSym( "%GfxModeExists%width%height%depth",bbGfxModeExists );
	rtSym( "%GfxModeWidth%mode",bbGfxModeWidth );
	rtSym( "%GfxModeHeight%mode",bbGfxModeHeight );
	rtSym( "%GfxModeDepth%mode",bbGfxModeDepth );
	rtSym( "%AvailVidMem",bbAvailVidMem );
	rtSym( "%TotalVidMem",bbTotalVidMem );
	rtSym( "%GfxDriver3D%driver",bbGfxDriver3D );
	rtSym( "%CountGfxModes3D",bbCountGfxModes3D );
	rtSym( "%GfxMode3DExists%width%height%depth",bbGfxMode3DExists );
	rtSym( "%GfxMode3D%mode",bbGfxMode3D );
	rtSym( "%Windowed3D",bbWindowed3D );
	rtSym( "Graphics%width%height%depth=0%mode=0",bbGraphics );
	rtSym( "Graphics3D%width%height%depth=0%mode=0",bbGraphics3D );
	rtSym( "EndGraphics",bbEndGraphics );
	rtSym( "%GraphicsLost",bbGraphicsLost );
	rtSym( "%GraphicsOpen",bbGraphicsOpen );
	rtSym( "SetGamma%src_red%src_green%src_blue#dest_red#dest_green#dest_blue",bbSetGamma );
	rtSym( "UpdateGamma%calibrate=0",bbUpdateGamma );
	rtSym( "#GammaRed%red",bbGammaRed );
	rtSym( "#GammaGreen%green",bbGammaGreen );
	rtSym( "#GammaBlue%blue",bbGammaBlue );
	rtSym( "%FrontBuffer",bbFrontBuffer );
	rtSym( "%BackBuffer",bbBackBuffer );
	rtSym( "%ScanLine",bbScanLine );
	rtSym( "VWait%frames=1",bbVWait );
	rtSym( "Flip%vwait=1",bbFlip );
	rtSym( "%GraphicsWidth",bbGraphicsWidth );
	rtSym( "%GraphicsHeight",bbGraphicsHeight );
	rtSym( "%GraphicsDepth",bbGraphicsDepth );
	rtSym( "SetBuffer%buffer",bbSetBuffer );
	rtSym( "%GraphicsBuffer",bbGraphicsBuffer );
	rtSym( "%LoadBuffer%buffer$bmpfile",bbLoadBuffer );
	rtSym( "%SaveBuffer%buffer$bmpfile",bbSaveBuffer );
	rtSym( "BufferDirty%buffer",bbBufferDirty );
	rtSym( "LockBuffer%buffer=0",bbLockBuffer );
	rtSym( "UnlockBuffer%buffer=0",bbUnlockBuffer );
	rtSym( "%ReadPixel%x%y%buffer=0",bbReadPixel );
	rtSym( "WritePixel%x%y%argb%buffer=0",bbWritePixel );
	rtSym( "%ReadPixelFast%x%y%buffer=0",bbReadPixelFast );
	rtSym( "WritePixelFast%x%y%argb%buffer=0",bbWritePixelFast );
	rtSym( "CopyPixel%src_x%src_y%src_buffer%dest_x%dest_y%dest_buffer=0",bbCopyPixel );
	rtSym( "CopyPixelFast%src_x%src_y%src_buffer%dest_x%dest_y%dest_buffer=0",bbCopyPixelFast );
	rtSym( "CopyRect%source_x%source_y%width%height%dest_x%dest_y%src_buffer=0%dest_buffer=0",bbCopyRect );
	rtSym( "%LoadImage$bmpfile",bbLoadImage );
	rtSym( "%LoadAnimImage$bmpfile%cellwidth%cellheight%first%count",bbLoadAnimImage );
	rtSym( "%CopyImage%image",bbCopyImage );
	rtSym( "%CreateImage%width%height%frames=1",bbCreateImage );
	rtSym( "FreeImage%image",bbFreeImage );
	rtSym( "%SaveImage%image$bmpfile%frame=0",bbSaveImage );
	rtSym( "GrabImage%image%x%y%frame=0",bbGrabImage );
	rtSym( "%ImageBuffer%image%frame=0",bbImageBuffer );
	rtSym( "DrawImage%image%x%y%frame=0",bbDrawImage );
	rtSym( "DrawBlock%image%x%y%frame=0",bbDrawBlock );
	rtSym( "TileImage%image%x=0%y=0%frame=0",bbTileImage );
	rtSym( "TileBlock%image%x=0%y=0%frame=0",bbTileBlock );
	rtSym( "DrawImageRect%image%x%y%rect_x%rect_y%rect_width%rect_height%frame=0",bbDrawImageRect );
	rtSym( "DrawBlockRect%image%x%y%rect_x%rect_y%rect_width%rect_height%frame=0",bbDrawBlockRect );
	rtSym( "MaskImage%image%red%green%blue",bbMaskImage );
	rtSym( "HandleImage%image%x%y",bbHandleImage );
	rtSym( "MidHandle%image",bbMidHandle );
	rtSym( "AutoMidHandle%enable",bbAutoMidHandle );
	rtSym( "%ImageWidth%image",bbImageWidth );
	rtSym( "%ImageHeight%image",bbImageHeight );
	rtSym( "%ImageXHandle%image",bbImageXHandle );
	rtSym( "%ImageYHandle%image",bbImageYHandle );
	rtSym( "ScaleImage%image#xscale#yscale",bbScaleImage );
	rtSym( "ResizeImage%image#width#height",bbResizeImage );
	rtSym( "RotateImage%image#angle",bbRotateImage );
	rtSym( "TFormImage%image#a#b#c#d",bbTFormImage );
	rtSym( "TFormFilter%enable",bbTFormFilter );
	rtSym( "%ImagesOverlap%image1%x1%y1%image2%x2%y2",bbImagesOverlap );
	rtSym( "%ImagesCollide%image1%x1%y1%frame1%image2%x2%y2%frame2",bbImagesCollide );
	rtSym( "%RectsOverlap%x1%y1%width1%height1%x2%y2%width2%height2",bbRectsOverlap );
	rtSym( "%ImageRectOverlap%image%x%y%rect_x%rect_y%rect_width%rect_height",bbImageRectOverlap );
	rtSym( "%ImageRectCollide%image%x%y%frame%rect_x%rect_y%rect_width%rect_height",bbImageRectCollide );
	rtSym( "Write$string",bbWrite );
	rtSym( "Print$string=\"\"",bbPrint );
	rtSym( "$Input$prompt=\"\"",bbInput );
	rtSym( "Locate%x%y",bbLocate );
}
