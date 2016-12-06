Graphics 640,480,0,1
SetBuffer BackBuffer()

Text 0,0,"EndGraphics Example Stage 1/2"
Text 0,20,"Currently in Graphics mode"
Text 0,40,"Press a key to deactive Graphics mode with EndGraphics command"

Flip

WaitKey()

EndGraphics

Print "EndGraphics Example Stage 2/2"
Print "Currently in non-Graphics mode"

WaitKey()

End