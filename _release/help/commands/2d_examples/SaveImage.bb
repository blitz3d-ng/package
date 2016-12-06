; SaveImage example

; Set Graphics Mode
Graphics 800,600,16

; Load an image to tile (your location might vary)
gfxBall=LoadImage("C:Program Files\Blitz Basic\samples\ball.bmp")

; Save the image to the c: drive ...
ok=SaveImage (gfxBall,"c:
ewball.bmp")

; Print results
If ok=1 Then 
Print "Save successful!"
Else
Print "There was an error saving!"
End If

; Wait for ESC to hit
While Not KeyHit(1)
Wend