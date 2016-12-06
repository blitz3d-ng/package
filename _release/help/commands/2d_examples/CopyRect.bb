; CopyRect Example

; Turn on graphics mode
Graphics 800,600,16

; create a blank image
gfxBlank=CreateImage (300,300)

; Fill the screen with random boxes in random colors
For t = 1 To 1000
Rect Rand(800),Rand(600),Rand(100),Rand(100),Rand(0,1) 
Color Rand(255),Rand(255),Rand(255)
Next

; Wait a couple of seconds so the user can see it
Delay 2000

; Copy graphics randomly from the front buffer to the blank image
CopyRect Rand(800),Rand(600),300,300,0,0,FrontBuffer(),ImageBuffer(gfxBlank)

; Clear the screen, draw the copied to image, wait for user to hit a key
Cls
DrawImage gfxBlank,0,0
WaitKey 
