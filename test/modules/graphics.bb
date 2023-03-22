
Context "Graphics"

SetRenderer "OpenGL"
Graphics 800,600,32,2

Expect GraphicsWidth()=800,"800px"
Expect GraphicsHeight()=600,"600px"

; now for some drawing...
Rect 10,10,100,100
Oval 10,10,100,100

logo=LoadImage( "../_release/samples/mak/b3dlogo.jpg" )
Expect logo<>0, "Can load a file"
Expect ImageWidth(logo)=400, "width of 400"
Expect ImageHeight(logo)=197, "height of 197"

Flip
