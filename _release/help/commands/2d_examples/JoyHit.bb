; JoyHit Example

; Set up the timer
current=MilliSecs()
Print "Press FireButton 1 a bunch of times for five seconds..."

; Wait 5 seconds
While MilliSecs() < current+5000
Wend

; Print the results
Print "Pressed button " + JoyHit(1) + " times."
