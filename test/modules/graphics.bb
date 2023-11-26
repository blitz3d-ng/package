
Context "Graphics"

Expect CountRenderers()>0, "There is at least one renderer"
Expect RendererName(0)<>"", "The first renderer has a name"

Expect CountGfxDrivers()>0, "There is at least one graphics driver"

AppTitle "hello, world"

SetRenderer "OpenGL"
Graphics 800,600,32,2

ExpectInt GraphicsWidth(),800,"800px"
ExpectInt GraphicsHeight(),600,"600px"
ExpectInt GraphicsLost(),False,"graphics not lost"

Locate 20,20
Print ""
Print "hello, world"
Write "yo"

Graphics 1280,720,32,2

AppTitle "hello, earth"

ExpectInt StringWidth("hello, world"), 84
ExpectInt StringWidth("hello, wörld"), 84

ClsColor 255,0,0
Cls

; now for some drawing...
Rect 10,10,100,100,True
Rect 10,10,100,100,False
Oval 10,10,100,100
Text 0,0,"hello, world"
Plot 123,321

logo=LoadImage( "../_release/samples/mak/b3dlogo.jpg" )
Expect logo<>0, "Can load an image"
ExpectInt ImageWidth(logo),400, "width of 400"
ExpectInt ImageHeight(logo),197, "height of 197"

TileImage logo,0,0

SetFont LoadFont("verdana", 12)
Text 0,0,"hello, world"

SetFont LoadFont("../_release/cfg/Blitz.fon", 12)
Text 0,0,"hello, world"

Flip
