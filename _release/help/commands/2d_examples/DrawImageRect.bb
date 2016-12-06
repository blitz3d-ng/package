; DrawImageRect Example

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
SetBuffer FrontBuffer()


; Let's draw portions of the circle randomly
While Not KeyHit(1)
; We take random sized portions of the circle and put them
; at a random location ... wash, rinse, and repeat
DrawImageRect gfxCircle,Rnd(640),Rnd(480),0,0,Rnd(50),Rnd(50),0
Delay 100
Wend
