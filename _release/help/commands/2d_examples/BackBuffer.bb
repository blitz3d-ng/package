; Flip/Backbuffer()/Rect Example

; Set Graphics Mode
Graphics 640,480

; Go double buffering
SetBuffer BackBuffer()

; Setup initial locations for the box
box_x = -20 ; negative so it will start OFF screen
box_y = 100

While Not KeyHit(1)
Cls ; Always clear screen first
Rect box_x,box_y,20,20,1 ; Draw the box in the current x,y location
Flip ; Flip it into view
box_x = box_x + 1 ; Move the box over one pixel
If box_x = 640 Then box_x=-20 ; If it leaves the Right edge, reset its x location
Wend
