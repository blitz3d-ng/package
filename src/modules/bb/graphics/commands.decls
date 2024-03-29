; renderer
CountRenderers%():"bbCountRenderers"
RendererName$( renderer% ):"bbRendererName"
SetRenderer%( name$ ):"bbSetRenderer"

;gfx driver info
CountGfxDrivers%():"bbCountGfxDrivers"
GfxDriverName$( driver% ):"bbGfxDriverName"
SetGfxDriver( driver% ):"bbSetGfxDriver"

;gfx mode info
CountGfxModes%():"bbCountGfxModes"
GfxModeExists%( width%,height%,depth% ):"bbGfxModeExists"

GfxModeWidth%( mode% ):"bbGfxModeWidth"
GfxModeHeight%( mode% ):"bbGfxModeHeight"
GfxModeDepth%( mode% ):"bbGfxModeDepth"
AvailVidMem%():"bbAvailVidMem"
TotalVidMem%():"bbTotalVidMem"

GfxDriver3D%( driver% ):"bbGfxDriver3D"
CountGfxModes3D%():"bbCountGfxModes3D"
GfxMode3DExists%( width%,height%,depth% ):"bbGfxMode3DExists"
GfxMode3D%( mode% ):"bbGfxMode3D"
Windowed3D%():"bbWindowed3D"

;display mode
Graphics( width%,height%,depth%=0,mode%=0 ):"bbGraphics"
Graphics3D( width%,height%,depth%=0,mode%=0 ):"bbGraphics3D"
EndGraphics():"bbEndGraphics"
GraphicsLost%():"bbGraphicsLost"
GraphicsOpen%():"bbGraphicsOpen"

SetGamma( src_red%,src_green%,src_blue%,dest_red#,dest_green#,dest_blue# ):"bbSetGamma"
UpdateGamma( calibrate%=0 ):"bbUpdateGamma"
GammaRed#( red% ):"bbGammaRed"
GammaGreen#( green% ):"bbGammaGreen"
GammaBlue#( blue% ):"bbGammaBlue"

FrontBuffer.BBCanvas():"bbFrontBuffer"
BackBuffer.BBCanvas():"bbBackBuffer"
ScanLine%():"bbScanLine"
VWait( frames%=1 ):"bbVWait"
Flip( vwait%=1 ):"bbFlip"
GraphicsWidth%():"bbGraphicsWidth"
GraphicsHeight%():"bbGraphicsHeight"
GraphicsDepth%():"bbGraphicsDepth"

;buffer management
SetBuffer( buffer.BBCanvas ):"bbSetBuffer"
GraphicsBuffer.BBCanvas():"bbGraphicsBuffer"
LoadBuffer%( buffer.BBCanvas,bmpfile$ ):"bbLoadBuffer"
SaveBuffer%( buffer.BBCanvas,bmpfile$ ):"bbSaveBuffer"
BufferDirty( buffer.BBCanvas ):"bbBufferDirty"

;fast pixel reads/write
LockBuffer( buffer.BBCanvas=0 ):"bbLockBuffer"
UnlockBuffer( buffer.BBCanvas=0 ):"bbUnlockBuffer"
ReadPixel%( x%,y%,buffer.BBCanvas=0 ):"bbReadPixel"
WritePixel( x%,y%,argb%,buffer.BBCanvas=0 ):"bbWritePixel"
ReadPixelFast%( x%,y%,buffer.BBCanvas=0 ):"bbReadPixelFast"
WritePixelFast( x%,y%,argb%,buffer.BBCanvas=0 ):"bbWritePixelFast"
CopyPixel( src_x%,src_y%,src_buffer.BBCanvas,dest_x%,dest_y%,dest_buffer.BBCanvas=0 ):"bbCopyPixel"
CopyPixelFast( src_x%,src_y%,src_buffer.BBCanvas,dest_x%,dest_y%,dest_buffer.BBCanvas=0 ):"bbCopyPixelFast"

CopyRect( source_x%,source_y%,width%,height%,dest_x%,dest_y%,src_buffer.BBCanvas=0,dest_buffer.BBCanvas=0 ):"bbCopyRect"

;rendering
Origin( x%,y% ):"bbOrigin"
Viewport( x%,y%,width%,height% ):"bbViewport"
Color( red%,green%,blue% ):"bbColor"
GetColor( x%,y% ):"bbGetColor"
ColorRed%():"bbColorRed"
ColorGreen%():"bbColorGreen"
ColorBlue%():"bbColorBlue"
ClsColor( red%,green%,blue% ):"bbClsColor"
Cls():"bbCls"
Plot( x%,y% ):"bbPlot"
Rect( x%,y%,width%,height%,solid%=1 ):"bbRect"
Oval( x%,y%,width%,height%,solid%=1 ):"bbOval"
Line( x1%,y1%,x2%,y2% ):"bbLine"
Text( x%,y%,text$,centre_x%=0,centre_y%=0 ):"bbText"

;fonts
LoadFont.BBFont( fontname$,height%=12,bold%=0,italic%=0,underline%=0 ):"bbLoadFont"
FreeFont( font.BBFont ):"bbFreeFont"
SetFont( font.BBFont ):"bbSetFont"
FontWidth%():"bbFontWidth"
FontHeight%():"bbFontHeight"
StringWidth%( string$ ):"bbStringWidth"
StringHeight%( string$ ):"bbStringHeight"

;movies
OpenMovie.BBMovie( file$ ):"bbOpenMovie"
DrawMovie%( movie.BBMovie,x%=0,y%=0,w%=-1,h%=-1 ):"bbDrawMovie"
MovieWidth%( movie.BBMovie ):"bbMovieWidth"
MovieHeight%( movie.BBMovie ):"bbMovieHeight"
MoviePlaying%( movie.BBMovie ):"bbMoviePlaying"
CloseMovie( movie.BBMovie ):"bbCloseMovie"

;images
LoadImage.BBImage( bmpfile$ ):"bbLoadImage"
LoadAnimImage.BBImage( bmpfile$,cellwidth%,cellheight%,first%,count% ):"bbLoadAnimImage"
CopyImage.BBImage( image.BBImage ):"bbCopyImage"
CreateImage.BBImage( width%,height%,frames%=1 ):"bbCreateImage"
FreeImage( image.BBImage ):"bbFreeImage"
SaveImage%( image.BBImage,bmpfile$,frame%=0 ):"bbSaveImage"

GrabImage( image.BBImage,x%,y%,frame%=0 ):"bbGrabImage"
ImageBuffer.BBCanvas( image.BBImage,frame%=0 ):"bbImageBuffer"
DrawImage( image.BBImage,x%,y%,frame%=0 ):"bbDrawImage"
DrawBlock( image.BBImage,x%,y%,frame%=0 ):"bbDrawBlock"
TileImage( image.BBImage,x%=0,y%=0,frame%=0 ):"bbTileImage"
TileBlock( image.BBImage,x%=0,y%=0,frame%=0 ):"bbTileBlock"
DrawImageRect( image.BBImage,x%,y%,rect_x%,rect_y%,rect_width%,rect_height%,frame%=0 ):"bbDrawImageRect"
DrawBlockRect( image.BBImage,x%,y%,rect_x%,rect_y%,rect_width%,rect_height%,frame%=0 ):"bbDrawBlockRect"
MaskImage( image.BBImage,red%,green%,blue% ):"bbMaskImage"
HandleImage( image.BBImage,x%,y% ):"bbHandleImage"
MidHandle( image.BBImage ):"bbMidHandle"
AutoMidHandle( enable% ):"bbAutoMidHandle"
ImageWidth%( image.BBImage ):"bbImageWidth"
ImageHeight%( image.BBImage ):"bbImageHeight"
ImageXHandle%( image.BBImage ):"bbImageXHandle"
ImageYHandle%( image.BBImage ):"bbImageYHandle"

ScaleImage( image.BBImage,xscale#,yscale# ):"bbScaleImage"
ResizeImage( image.BBImage,width#,height# ):"bbResizeImage"
RotateImage( image.BBImage,angle# ):"bbRotateImage"
TFormImage( image.BBImage,a#,b#,c#,d# ):"bbTFormImage"
TFormFilter( enable% ):"bbTFormFilter"

ImagesOverlap%( image1.BBImage,x1%,y1%,image2.BBImage,x2%,y2% ):"bbImagesOverlap"
ImagesCollide%( image1.BBImage,x1%,y1%,frame1%,image2.BBImage,x2%,y2%,frame2% ):"bbImagesCollide"
RectsOverlap%( x1%,y1%,width1%,height1%,x2%,y2%,width2%,height2% ):"bbRectsOverlap"
ImageRectOverlap%( image.BBImage,x%,y%,rect_x%,rect_y%,rect_width%,rect_height% ):"bbImageRectOverlap"
ImageRectCollide%( image.BBImage,x%,y%,frame%,rect_x%,rect_y%,rect_width%,rect_height% ):"bbImageRectCollide"

;console emu
Write( string$ ):"bbWrite"
Print( string$="" ):"bbPrint"
Input$( prompt$="" ):"bbInput"
Locate( x%,y% ):"bbLocate"
