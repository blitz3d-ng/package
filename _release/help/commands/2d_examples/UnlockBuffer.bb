; High Speed Graphics Commands

Graphics 640,480,16

; Draw a bunch of stuff on the screen
For t= 1 To 1000
Color Rnd(255),Rnd(255),Rnd(255)
Rect Rnd(640),Rnd(480),Rnd(150),Rnd(150),Rnd(1)
Next

Delay 3000

; Copy the top half of the screen over the bottom half
; using fast pixels and locked buffers
For x = 1 To 640
For y = 1 To 240
LockBuffer FrontBuffer()
WritePixelFast x,y+241,ReadPixelFast(x,y)
UnlockBuffer FrontBuffer()
Next
Next

Delay 3000

; Draw the left half of the screen over the right half
; using the slower direct pixel access
For x = 1 To 320
For y = 1 To 480
WritePixel x+320,y,ReadPixel(x,y)
Next
Next