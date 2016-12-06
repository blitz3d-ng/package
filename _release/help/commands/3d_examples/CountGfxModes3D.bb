; CountGfxModes3D()
; -----------------

Graphics 800,600,0,2

For i=1 To CountGfxModes3D()

	Print "Mode "+i+": ("+GfxModeWidth(i)+"x"+GfxModeHeight(i)+"x"+GfxModeDepth(i)+")"

Next

WaitKey()