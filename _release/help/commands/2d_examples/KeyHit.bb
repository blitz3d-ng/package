; KeyHit Example

; Set up the timer
current=MilliSecs()
Print "Press ESC a bunch of times for five seconds..."

; Wait 5 seconds
While MilliSecs() < current+5000
Wend

; Print the results
Print "Pressed ESC " + KeyHit(1) + " times."
