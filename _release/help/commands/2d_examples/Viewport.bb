; ViewPort Example

; Set Up Graphics Mode
Graphics 800,600

; Set up Double Buffering
SetBuffer BackBuffer()

; Set viewport starting at 100,100 and make it 200,200 pixels in size
Viewport 100,100,200,200

; Infinately draw random rectangles with random colors
While Not KeyHit(1)
Cls ; Clear screen in 'blitting' technique
For t = 1 To 100 ; Do 100 rectangles each time
Color Rnd(255),Rnd(255),Rnd(255) ; Random color
Rect Rnd(800),Rnd(600),Rnd(300),Rnd(300),Rnd(0,1) ; Random sized and located  box, some filled
Next ; repeat that drawing loop
Flip ; Flip to the back buffer
Wend
