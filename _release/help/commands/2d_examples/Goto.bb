Print "The program starts ..."
Goto label1
Print "This line never gets printed .."
End

.label1
Print "We just jumped here!"

; wait for ESC key before ending
While Not KeyHit(1) 
Wend