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
