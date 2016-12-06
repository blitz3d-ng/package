; GraphicsWidth(), GraphicsHeight(), GraphicsDepth(), GraphicsBuffer() example

; Set a graphics mode and buffer
Graphics 640,480,16
SetBuffer FrontBuffer()

; Print the details
Print "Screen width is: " + GraphicsWidth()
Print "Screen height is: " + GraphicsHeight()
Print "Screen color depth is: " + GraphicsDepth()
Print "Current buffer handle is: " + GraphicsBuffer()

; Wait for ESC before exiting
While Not KeyHit(1)
Wend
