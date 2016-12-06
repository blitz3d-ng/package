; GrabImage example

; Turn on graphics
Graphics 640,480,16

; We'll be drawing right to the front buffer
SetBuffer FrontBuffer()

; You must create an empty image to grab from first
gfxGrab=CreateImage(100,100)

; Draw random rectangles on the screen until the
; user presses ESC
While Not KeyHit(1)
; random color
Color Rnd(255),Rnd(255),Rnd(255)
; super random rectangles
Rect Rnd(640),Rnd(480),Rnd(100),Rnd(100),Rnd(1)
Delay 50
Wend

; Now, grab an image, starting at 100,100 and put it in gfxGrab
GrabImage gfxGrab,100,100

; Clear screen and show user the grabbed image
Cls
Text 0,0, "Here is your grabbed image! Press a mouse key ..."
DrawImage gfxgrab,50,50

; Wait for a mouse press
WaitMouse()
