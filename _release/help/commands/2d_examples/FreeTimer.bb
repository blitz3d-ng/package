; Create the timer to track speed
frameTimer=CreateTimer(60)

; Your main screen draw loop
While Not KeyHit(1)
WaitTimer(frameTimer) ; Pause until the timer reaches 60
Cls
; Draw your screen stuff
Flip
Wend

; Kill the timer
FreeTimer(frameTimer)
