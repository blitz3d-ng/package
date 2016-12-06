; GFXModeExists example

; If there is a mode, tell user
mode=GfxModeExists(800,800,16)

If mode=1 Then 
Print "The mode you requested exists!"
Else
Print "Sorry, that mode doesn't exist." 
End If

; Wait for ESC press from user
While Not KeyHit(1)
Wend
