; FrontBuffer()/Rect Example

; Engage graphics mode
Graphics 640,480,16

; Set the drawing buffer to front - instant drawing ops!
SetBuffer FrontBuffer()

; Repeat this until user presses ESC
While Not KeyHit(1)
; Set a random color
Color Rnd(255),Rnd(255),Rnd(255)
; Draw a rectangle at a random location, with a random width and height
; plus randomly choose if the rectangle is solid or just an outline
Rect Rnd(640),Rnd(480),Rnd(50),Rnd(50),Rnd(0,1)
; Blitz is so dang fast, we need a delay so you can watch it draw!
Delay 10
Wend