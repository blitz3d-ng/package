; ImageHeight/ImageWidth Example

; Global, as always, for graphics
Global gfxPlayer

; Enter graphics mode and start double buffering
Graphics 640,480,16
SetBuffer BackBuffer()

; Load the image-assign the handle to gfxPlayer
gfxPlayer=LoadImage("player.bmp")

; Print the image dimensions
Print "The image height is: " + ImageHeight(gfxPlayer)
Print "The image width is: " + ImageWidth(gfxPlayer)

; Wait until ESC is pressed so you can see the output
While Not KeyHit(1)
Wend
