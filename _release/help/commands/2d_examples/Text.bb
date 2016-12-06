; Text example

; enable graphics mode
Graphics 800,600,16

; wait for ESC key before ending
While Not KeyHit(1)
;print the text, centered horizontally at x=400, y=0
Text 400,0,"Hello There!",True,False
Wend
