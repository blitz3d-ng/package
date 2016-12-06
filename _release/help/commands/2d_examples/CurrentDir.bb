; CurrentDir$() example

; Print the current directory until ESC key
While Not KeyHit(1)
Print CurrentDir$()
Wend