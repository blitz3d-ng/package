; Local example

; set lives to 5 for the main program loop
Local lives=5

; Call a function
while not keyhit(1)
showlives()
Wend

Function showlives()
; For this function, lives will be 10!
Local lives=10
Print lives
End Function