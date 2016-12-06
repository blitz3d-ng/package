; TileBlock example
Graphics 800,600,16

; Load an image to tile (your location might vary)
gfxBall=LoadImage("C:Program FilesBlitz Basicsamplesall.bmp")

; Tile the graphic without transparency
TileBlock gfxBall

; Wait for ESC to hit
While Not KeyHit(1)
Wend
