Print "The program starts ..."
Gosub label1
Print "The Program ends ..."

; wait for ESC key before ending
While Not KeyHit(1) 
Wend

End

.label1
Print "We could do all sorts of things in this part of the program..."
Print "But, we'll just go back to the original code, instead ..."
Return