Graphics3D 1280,720,32,2
SetBuffer BackBuffer()
SeedRnd MilliSecs()

; create add texture - white cirlce on a black background
blob_tex=CreateTexture(256,256)
SetBuffer TextureBuffer(blob_tex)
For n=0 To 50
	Color 5+(n*5),5+(n*5),5+(n*5)
	Oval 10+n,10+n,236-(n*2),236-(n*2),1
Next

; create cube texture
cube_tex=CreateTexture(256,256)
SetBuffer TextureBuffer(cube_tex)
Color 255,255,255
Rect 0,0,256,256,1
For n=0 To 7
	If n=0 Then Color 0,0,0
	If n=1 Then Color 0,0,255
	If n=2 Then Color 0,255,0
	If n=3 Then Color 0,255,255
	If n=4 Then Color 255,0,0
	If n=5 Then Color 255,0,255
	If n=6 Then Color 255,255,0
	If n=7 Then Color 255,255,255
	Rect n*32,n*32,32,32,1
Next
Color 0,0,0
For n=0 To 255 Step 32
	Line 0,n,255,n
	Line n,0,n,255
Next

SetBuffer BackBuffer()

; create camera
camera=CreateCamera()
CameraClsColor camera,160,160,160
PositionEntity camera,0,0,-35
middle=CreatePivot()
EntityParent camera,middle

; create cube
cube=CreateCube()
ScaleEntity cube,11,11,11
EntityTexture cube,cube_tex
EntityFX cube,17 ;set fullbright and 2 sided textures
EntityBlend cube,2 ;set multiply blend

; create blobs using add blend mode
max_blobs=100
Dim blobs(max_blobs) ; blob sprites
Dim xyblobs#(max_blobs,2) ; blob vector

For n=0 To max_blobs
	blobs(n)=CreateSprite()
	EntityFX blobs(n),1
	EntityBlend blobs(n),3 ;set blend mode to add
	EntityTexture blobs(n),blob_tex
	xyblobs(n,0)=Rnd(-.1,.1)
	xyblobs(n,1)=Rnd(-.1,.1)
	xyblobs(n,2)=Rnd(-.1,.1)
	EntityColor blobs(n),Rand(0,255),Rand(0,255),Rand(0,255) ;give it a colour
Next

Repeat
	; move the blobs around
	For n=0 To max_blobs
		MoveEntity blobs(n),xyblobs(n,0),xyblobs(n,1),xyblobs(n,2)
		;bounce off sides
		If EntityX(blobs(n))<-10 Or EntityX(blobs(n))>10 Then xyblobs(n,0)=-xyblobs(n,0)
		If EntityY(blobs(n))<-10 Or EntityY(blobs(n))>10 Then xyblobs(n,1)=-xyblobs(n,1)
		If EntityZ(blobs(n))<-10 Or EntityZ(blobs(n))>10 Then xyblobs(n,2)=-xyblobs(n,2)
	Next

	; turn camera
	TurnEntity middle,.1,.2,.3

	UpdateWorld
	RenderWorld
	Flip
Until KeyHit(1)
