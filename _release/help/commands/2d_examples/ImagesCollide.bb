; ImagesCollide Example

; Turn on graphics mode
Graphics 640,480,16

; Create two new empty graphics to store our circle and box in
gfxBox=CreateImage(50,50)
gfxCircle=CreateImage(50,50)

; Draw the box image first
; Set drawing operations to point to our new empty graphic
SetBuffer ImageBuffer(gfxBox)
; Change drawing color to blue
Color 0,0,255
;Draw our box (note that it has a 10 pixel space around it)
Rect 10,10,30,30,1

; Repeat for the circle graphic
SetBuffer ImageBuffer(gfxCircle)
Color 255,0,0
; Note the extra space between the circle and the edge of the graphic
Oval 10,10,30,30,1

; Let's not forget to put the drawing buffer back!
SetBuffer BackBuffer()

; Locate our box to a random, visible screen location
boxX=Rnd(50,610)
boxY=Rnd(50,430)

; Repeat the loop until we've had a collision
Repeat 
; Attach our mouse to the circle to move it
circleX=MouseX()
circleY=MouseY()
; Standard double buffer technique; clear screen first
Cls
; Draw our objects at the designated location
DrawImage gfxBox,boxX,boxY
DrawImage gfxCircle,circleX,circleY
; Standard double buffer technique; flip after all drawing is done
Flip
; We test the locations of our box and circle to see if they have pixel collided
Until ImagesCollide (gfxBox,boxX,boxY,0,gfxCircle,circleX,circleY,0)

; Loop is over, we must've collided!
Text 0,0, "WE'VE HAD A COLLISION! PRESS A MOUSE BUTTON"
; Can't see the text until we flip ..
Flip
; Wait for a mouse click
WaitMouse()
; End our graphics mode
EndGraphics