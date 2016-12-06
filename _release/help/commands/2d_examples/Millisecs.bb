; This prints STILL WAITING! for three seconds then ends.
oldTime=MilliSecs()
While MilliSecs() < oldTime + 3000
Print "Still waiting!"
Wend
