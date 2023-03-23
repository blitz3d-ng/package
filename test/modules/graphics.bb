
Context "Graphics"

SetRenderer "OpenGL"
Graphics 800,600,32,2

Expect GraphicsWidth()=800,"800px"
Expect GraphicsHeight()=600,"600px"
Expect GraphicsLost()=false,"graphics not lost"

; now for some drawing...
Rect 10,10,100,100,True
Rect 10,10,100,100,False
Oval 10,10,100,100
Text 0,0,"hello, world"
Plot 123,321

logo=LoadImage( "../_release/samples/mak/b3dlogo.jpg" )
Expect logo<>0, "Can load a file"
Expect ImageWidth(logo)=400, "width of 400"
Expect ImageHeight(logo)=197, "height of 197"

Flip
