; CopyPixel/CopyPixelFast Example
; -------------------------------

Graphics 640,480,16

Print "Press a key to use CopyPixel to copy the top half of an image to the  frontbuffer"
WaitKey()

; Load an image - can be anything
pic=LoadImage("media/blitz_pic.bmp")

; Use CopyPixel to copy the top half of the image to the frontbuffer
For y=0 To ImageHeight(pic)/2
For x=0 To ImageWidth(pic)
CopyPixel x,y,ImageBuffer(pic),x,y
Next
Next

Locate 0,GraphicsHeight()/2
Print "Press a key to use CopyPixelFast to copy the bottom half of the image"
Print "Once this has finished, you can then press a key to end the program"

WaitKey()

; Lock buffer before using CopyPixelFast
LockBuffer

; Use CopyPixelFast to copy the bottom half of the image to the frontbuffer
For y=0 To (ImageHeight(pic)/2)+ImageHeight(pic)
For x=0 To ImageWidth(pic)
CopyPixelFast x,y,ImageBuffer(pic),x,y
Next
Next

; Unlock buffer after using CopyPixelFast
UnlockBuffer

WaitKey()