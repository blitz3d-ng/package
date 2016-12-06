; DrawBlock Example

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
; Set the CLS color to white
ClsColor 255,255,255

; Let the user move the circle graphic around a white screen
; putting the graphic at the MouseX,Y coordinates
While Not KeyHit(1)
Cls
DrawBlock gfxCircle,MouseX(),MouseY()
Flip
Wend