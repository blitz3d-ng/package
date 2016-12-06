Graphics 640,480

SetBuffer BackBuffer()

Repeat
Cls

Text 320,0,"Click to reset mouse",True

Text 0,0,"Mouse X:"+MouseX()
Text 0,10,"Mouse Y:"+MouseY()

If MouseDown(1) Or MouseDown(2) Then MoveMouse 320,240

Text MouseX(),MouseY(),"X",True,True

Flip

Until KeyHit(1)

End