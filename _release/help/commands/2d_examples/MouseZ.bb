Graphics 640, 480, 0, 2

SetBuffer BackBuffer ()

Repeat
Flip:Cls
Text 20, 20, "Mouse wheel position: " + MouseZ ()
Until KeyHit (1)

End