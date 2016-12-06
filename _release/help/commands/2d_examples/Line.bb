; Line example
Graphics 800,600,16

; Wait for ESC to hit
While Not KeyHit(1)
; Set a random color
Color Rnd(255),Rnd(255),Rnd(255)
; Draw a random line
Line Rnd(800),Rnd(600),Rnd(800),Rnd(600)
Wend
