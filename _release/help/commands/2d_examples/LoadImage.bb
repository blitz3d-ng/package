; LoadImage and DrawImage example

; Declare a variable to hold the graphic file handle
Global gfxPlayer

; Set a graphics mode
Graphics 640,480,16

; Set drawing operations for double buffering
SetBuffer BackBuffer()

; Load the image and assign its file handle to the variable
; - This assumes you have a graphic called player.bmp in the
; same folder as this source code
gfxPlayer=LoadImage("player.bmp")

; Let's do a loop where the graphic is drawn wherever the
; mouse is pointing. ESC will exit.
While Not KeyHit(1)
Cls ; clear the screen
DrawImage gfxPlayer,MouseX(),MouseY() ; Draw the image!
Flip ; flip the image into view and clear the back buffer
Wend