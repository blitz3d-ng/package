; LoadBuffer example

; Set graphics mode
Graphics 800,600,16

; Load an image directly to the front buffer (your location may be different)
LoadBuffer (FrontBuffer(),"C:Program FilesBlitz Basicsampleslitzanoidgfx	itle.bmp")

; wait for ESC so user gets to see the screen
While Not KeyHit(1)
Wend 
