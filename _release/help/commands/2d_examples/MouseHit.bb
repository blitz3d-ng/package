; MouseHit Example

; Set up the timer
current=MilliSecs()
Print "Press left mouse button a bunch of times for five seconds..."

; Wait 5 seconds
While MilliSecs() < current+5000
Wend

; Print the results
Print "Pressed left button " + MouseHit(1) + " times."
