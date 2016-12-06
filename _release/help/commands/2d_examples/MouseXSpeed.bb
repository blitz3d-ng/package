Graphics 640,480

SetBuffer BackBuffer()

x=320
y=240

; infinite mouse movement

Repeat
Cls

xs=MouseXSpeed() ; see how far the mouse has been moved
ys=MouseYSpeed()
MoveMouse 320,240 ;put the mouse back in the middle of the screen

x=x+xs ;adjust mouse co-ords
y=y+ys

If x>GraphicsWidth()-1 Then x=x-GraphicsWidth() ;wrap screen
If x<0 Then x=x+GraphicsWidth()
If y<0 Then y=y+GraphicsHeight()
If y>GraphicsHeight()-1 Then y=y-GraphicsHeight()

Text x,y,"X",True,True

Flip

Until KeyHit(1)

End