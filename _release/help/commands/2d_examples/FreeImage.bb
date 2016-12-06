; FreeImage command

; Global, as always, for graphics
Global gfxPlayer

; Enter graphics mode and start double buffering
Graphics 640,480,16
SetBuffer BackBuffer()

; Load the image-assign the handle to gfxPlayer
gfxPlayer=LoadImage("player.bmp")

; draw the image at random until ESC pressed
While Not KeyHit(1)
Cls
DrawImage gfxPlayer,Rnd(640),Rnd(480)
Flip
Wend

; Erase the image from memory!
FreeImage gfxPlayer