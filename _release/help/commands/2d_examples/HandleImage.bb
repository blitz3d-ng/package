;HandleImage Example

Graphics 800,600,16

gfxPlayer=LoadImage("player.bmp")
HandleImage gfxPlayer,20,20
DrawImage gfxPlayer,0,0
WaitKey
