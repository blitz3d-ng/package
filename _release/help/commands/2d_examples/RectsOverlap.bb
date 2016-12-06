; RectsOverlap Example
; Flashing graphics warning! Gets hypnotic ...

; Turn on graphics mode
Graphics 640,480,16

; Double buffering, and randomize the randomizer
SetBuffer BackBuffer()
SeedRnd MilliSecs()

; Repeat the loop until ESC pressed
While Not KeyHit(1)

; Generate a random rectangle
rect1X=Rnd(50,610)
rect1Y=Rnd(50,430)
rect1W=20
rect1H=20

; And another
rect2X=Rnd(50,610)
rect2Y=Rnd(50,430)
rect2W=20
rect2H=20
; Clear the screen standard double buffering
Cls
; Draw our rectangle2 in random colors
Color Rnd(255),Rnd(255),Rnd(255)
Rect rect1X,rect1Y,rect1W,rect1H,0
Color Rnd(255),Rnd(255),Rnd(255)
Rect rect2X,rect2Y,rect2W,rect2H,0

; Did they collide? If so, print a message and exit the loop!
If RectsOverlap (rect1X,rect1Y,rect1W,rect1H,rect2X,rect2Y,rect2W,rect2H) Then
Text 0,0, "Our boxes finally collided! Press a mouse button..."
; We do a flip here to ensure the text message gets seen too!
Flip
Exit ; exit the While/Wend loop
End If
; Flip the rects into view, wait 1/10th of a sec, repeat
Flip
Delay 100
Wend
; Wait for a mouse click
WaitMouse()
; End our graphics mode
EndGraphics
