; FontWidth()/FontHeight example

; enable graphics mode
Graphics 800,600,16

; Set global on font variable
Global fntArial

;Load fonts to a file handle variables
fntArial=LoadFont("Arial",13,False,False,False)

; set the font and print sizes
SetFont fntArial
Text 400,0,"The font width of the widest character is:"+ FontWidth(),True,False
Text 400,30,"The height of the font is:"+ FontHeight(),True,False

; Standard 'wait for ESC' from user
While Not KeyHit(1)
Wend

; Clear all the fonts from memory!
FreeFont fntArial
