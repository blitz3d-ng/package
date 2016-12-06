; ReadPixelFast/WritePixeFast Example
; -----------------------------------

Graphics 640,480,16

Print "Press a key to read color values"
WaitKey()

; Load and draw an image on to the screen - can be anything
pic=LoadImage("media/blitz_pic.bmp")
DrawImage pic,0,0 

; Initialise an array big enough to fit all the color information of the screen
Dim pix(GraphicsWidth(),GraphicsHeight())

; Lock buffer before using ReadPixelFast
LockBuffer

; Use ReadPixel to get all the color information of the screen
For y=0 To GraphicsHeight()
For x=0 To GraphicsWidth()
pix(x,y)=ReadPixelFast(x,y)
Next
Next

; Lock buffer after using ReadPixelFast
UnlockBuffer

Cls
Locate 0,0
Print "Press a key to write pixels"
Print "Once this has finished, you can then press a key to end the program"

WaitKey()

; Lock buffer before using WritePixelFast
LockBuffer

; Use WritePixel to redraw the screen using the color information we got earlier
For y=0 To GraphicsHeight()
For x=0 To GraphicsWidth()
WritePixelFast x,y,pix(x,GraphicsHeight()-y) ; get y array value in backwards  order, to flip screen
Next
Next

; Unlock buffer after using WritePixelFast
UnlockBuffer

WaitKey()
