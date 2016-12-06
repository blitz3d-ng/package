; ScaleImage example

; Set Graphics Mode
Graphics 800,600,16

; Randomize the random seed
SeedRnd MilliSecs()

; Load an image to tile (your location might vary)
gfxBall=LoadImage("C:Program FilesBlitz Basicsamplesall.bmp")

; Scale it randomly from 50% to 150% both x and y
ScaleImage gfxBall,Rnd(-2.0,2.0),Rnd(-2.0,2.0)

; Wait for ESC to hit
While Not KeyHit(1)
DrawImage gfxball,Rnd(800),Rnd(600)
VWait 
Wend
