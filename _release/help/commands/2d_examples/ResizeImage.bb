; ResizeImage example

; Set Graphics Mode
Graphics 800,600,16

; Randomize the random seed
SeedRnd MilliSecs()

; Load an image to tile (your location might vary)
gfxBall=LoadImage("C:Program FilesBlitz Basicsamplesall.bmp")

; Size it randomly from 300 to -300 both x and y
ResizeImage gfxBall,Rnd(-300,300),Rnd(-300,300)

; Wait for ESC to hit
While Not KeyHit(1)
DrawImage gfxball,Rnd(800),Rnd(600)
VWait 
Wend
