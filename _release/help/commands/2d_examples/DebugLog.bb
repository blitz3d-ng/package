; DebugLog Example

; Let's start graphics mode
Graphics 640,480,16

; Now, let's load an image that doesn't exist!
gfxPlayer=LoadImage("noimagefound.jpg")
If gfxPlayer=0 Then
DebugLog "Player's Graphics failed to load!"
End If

; This is supposed to generate an error. Press F9 to see the log!
While Not KeyHit(1)
DrawImage gfxPlayer,100,100
Wend