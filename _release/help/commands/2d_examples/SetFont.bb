; LoadFont/SetFont/FreeFont example

; enable graphics mode
Graphics 800,600,16

; Set global on font variables
Global fntArial,fntArialB,fntArialI,fntArialU

;Load fonts to a file handle variables
fntArial=LoadFont("Arial",24,False,False,False)
fntArialB=LoadFont("Arial",18,True,False,False)
fntArialI=LoadFont("Arial",32,False,True,False)
fntArialU=LoadFont("Arial",14,False,False,True)

; set the font and print text
SetFont fntArial
Text 400,0,"This is just plain Arial 24 point",True,False

SetFont fntArialB
Text 400,30,"This is bold Arial 18 point",True,False

SetFont fntArialI
Text 400,60,"This is italic Arial 32 point",True,False

SetFont fntArialU
Text 400,90,"This is underlined Arial 14 point",True,False

; Standard 'wait for ESC' from user
While Not KeyHit(1)
Wend

; Clear all the fonts from memory!
FreeFont fntArial
FreeFont fntArialB
FreeFont fntArialI
FreeFont fntArialU
