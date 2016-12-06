; GetColor Example

Graphics 320,200
SetBuffer BackBuffer()

For t = 1 To 1000
Color Rnd(255), Rnd(255), Rnd(255)
Rect Rnd(320),Rnd(200),10,10,1
Next

GetColor 100,100
Print "Box at 100,100 is RGB:" + ColorRed() + "," + ColorGreen() + "," + ColorBlue()  + "!"
Flip 
While Not KeyHit(1)
Wend
