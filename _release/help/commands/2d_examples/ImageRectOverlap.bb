; ImageRectOverlap Example

; Turn on graphics mode
Graphics 640,480,16

; Create new empty graphic to store our circle in
gfxCircle=CreateImage(50,50)

; Draw the circle image
; Set drawing operations to point to our new empty graphic
SetBuffer ImageBuffer(gfxCircle)
Color 255,0,0
; Note the extra space between the circle and the edge of the graphic
Oval 10,10,30,30,1

; Let's not forget to put the drawing buffer back!
SetBuffer BackBuffer()
Color 0,0,255

; Locate our box to a random, visible screen location
hotX=Rnd(50,610)
hotY=Rnd(50,430)
hotW=Rnd(20,100)
hotH=Rnd(20,100)

; Repeat the loop until we've had a collision
Repeat 
; Attach our mouse to the circle to move it
circleX=MouseX()
circleY=MouseY()
; Standard double buffer technique; clear screen first
Cls
; Draw our rectangle
Rect hotX,hotY,hotW,hotH,0
DrawImage gfxCircle,circleX,circleY
; Standard double buffer technique; flip after all drawing is done
Flip
; We test the locations of our rectangle area and circle to see if they have  overlapped
Until ImageRectOverlap (gfxCircle,circleX,circleY,hotX,hotY,hotW,hotH)

; Loop is over, we must've collided!
Text 0,0, "WE'VE HAD A COLLISION! PRESS A MOUSE BUTTON"
; Can't see the text until we flip ..
Flip
; Wait for a mouse click
WaitMouse()
; End our graphics mode
EndGraphics
