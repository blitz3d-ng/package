; Origin example
Graphics 800,600,16

; Offset drawing options with origin command -200 in each direction
Origin -200,-200

; Wait for ESC to hit
While Not KeyHit(1)

; Draw an oval - SHOULD be at the exact center, but it isn't!
Oval 400,300,50,50,1
Wend
