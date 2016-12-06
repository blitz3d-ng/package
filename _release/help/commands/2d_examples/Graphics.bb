;GRAPHICS Example

; Set The Graphic Mode
Graphics 800,600

; Now print something on the graphic screen
Text 0,0, "This is some text printed on the graphic screen (and a white box)!  Press ESC ..."

; Now for a box
Rect 100,100,200,200,1

While Not KeyHit(1)
Wend