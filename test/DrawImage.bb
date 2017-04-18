

Graphics 800,600,32,2
SetBuffer BackBuffer()

img=LoadImage( "_release/samples/mak/b3dlogo.jpg" )

while Not KeyDown(1)
    Cls

    Color 255,255,255

    DrawImage img,10,10,0
    Rect 400,400,50,50,1

    Color 0,255,0
    Rect MouseX(),MouseY(),20,20

    Flip
Wend

End
