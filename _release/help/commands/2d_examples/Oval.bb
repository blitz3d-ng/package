; Oval example
Graphics 800,600,16

; Wait for ESC to hit
While Not KeyHit(1)
; Set a random color
Color Rnd(255),Rnd(255),Rnd(255)
; Draw a random oval
Oval Rnd(800),Rnd(600),Rnd(100),Rnd(100),Rnd(0,1)
Wend
