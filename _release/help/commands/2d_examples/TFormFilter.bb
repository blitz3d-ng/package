; RotateImage/TFormFilter Example

; Turn on graphics mode
Graphics 640,480,16

; Remove the line below to see the difference
; between filter on and off.
TFormFilter 0

; Create new empty graphic to store our circle in
gfxBox=CreateImage(50,50)

; Draw the box image
; Set drawing operations to point to our new empty graphic
SetBuffer ImageBuffer(gfxBox)
Color 255,0,0
; Note the extra space between the box and the edge of the graphic
Rect 10,10,30,30,1
SetBuffer FrontBuffer()

While Not KeyHit(1)
; Make a copy of the image so we are always using a fresh one each time
; we rotate it.
gfxTemp=CopyImage(gfxBox)
; Rotate it a random value and draw it at a random location
RotateImage gfxTemp,Rnd(360) 
DrawImage gfxTemp,Rnd(640),Rnd(480)
Wend
