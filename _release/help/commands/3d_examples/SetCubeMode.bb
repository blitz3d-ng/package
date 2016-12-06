; SetCubeMode Example
; -------------------

width=640
height=480
depth=0
mode=0

Graphics3D width,height,depth,mode
SetBuffer BackBuffer()

; If user's graphics card does not support cubic mapping then quit example
If GfxDriverCaps3D()<110 Then RuntimeError "Sorry, your graphics card does not support cubic environemnt maps."

cam=CreateCamera()
PositionEntity cam,0,10,-10

; Create separate camera for updating cube map - this allows us to manipulate main camera and cube camera which avoids any confusion
cube_cam=CreateCamera()
HideEntity cube_cam

light=CreateLight()
RotateEntity light,90,0,0

; Load object we will apply cubemap to - the classic teapot
teapot=LoadMesh("media/teapot.x")
ScaleEntity teapot,3,3,3
PositionEntity teapot,0,10,0

; Create some scenery

; ground
ground=CreatePlane()
EntityColor ground,168,133,55
ground_tex=LoadTexture("media/sand.bmp")
ScaleTexture ground_tex,10,10
EntityTexture ground,ground_tex

; sky
sky=CreateSphere(24)
ScaleEntity sky,500,500,500
FlipMesh sky
EntityFX sky,1
sky_tex=LoadTexture("media/sky.bmp")
EntityTexture sky,sky_tex

; cactus
cactus=LoadMesh("media/cactus2.x")
FitMesh cactus,-5,0,-5,2,6,.5

; camel
camel=LoadMesh("media/camel.x")
FitMesh camel,5,0,-5,6,5,4

; Load ufo to give us a dynamic moving object that the cubemap will be able to reflect
ufo_piv=CreatePivot()
PositionEntity ufo_piv,0,15,0
ufo=LoadMesh("media/green_ufo.x",ufo_piv)
PositionEntity ufo,0,0,10

; Create texture with color + cubic environment map + store in vram flags
tex=CreateTexture(256,256,1+128+256)

; Apply cubic environment map to teapot
EntityTexture teapot,tex

; Set initial cube mode value
cube_mode=1

While Not KeyDown(1)

	; Control camera
	
	; mouse look
	
	mxs#=mxs#+(MouseXSpeed()/5.0)
	mys#=mys#+(MouseYSpeed()/5.0)

	RotateEntity cam,mys#,-mxs#,0

	MoveMouse width/2,height/2

	; move camera forwards/backwards/left/right with cursor keys
	
	If KeyDown(200)=True Then MoveEntity cam,0,0,.2 ; move camera forward
	If KeyDown(208)=True Then MoveEntity cam,0,0,-.2 ; move camera back

	If KeyDown(205)=True Then MoveEntity cam,.2,0,0 ; move camera left
	If KeyDown(203)=True Then MoveEntity cam,-.2,0,0 ; move camera right
	
	; If M key pressed then change cube mode
	If KeyHit(50)
		cube_mode=cube_mode+1
		If cube_mode=4 Then cube_mode=1
		SetCubeMode tex,cube_mode
	EndIf
	
	; Turn ufo pivot, causing child ufo mesh to spin around it (and teapot)
	TurnEntity ufo_piv,0,2,0

	; Hide our main camera before updating cube map - we don't need it to be rendererd every time cube_cam is rendered
	HideEntity cam

	; Update cubemap
	UpdateCubemap(tex,cube_cam,teapot)

	; Show main camera again
	ShowEntity cam

	RenderWorld
	
	Text 0,0,"Use mouse to look around"
	Text 0,20,"Use cursor keys to change camera position"
	Text 0,40,"Press M to change cube mode"
	Text 0,60,"SetCubeMode tex,"+cube_mode
	
	Flip

Wend


Function UpdateCubemap(tex,camera,entity)

	tex_sz=TextureWidth(tex)

	; Show the camera we have specifically created for updating the cubemap
	ShowEntity camera
	
	; Hide entity that will have cubemap applied to it. This is so we can get cubemap from its position, without it blocking the view
	HideEntity entity

	; Position camera where the entity is - this is where we will be rendering views from for cubemap
	PositionEntity camera,EntityX#(entity),EntityY#(entity),EntityZ#(entity)

	CameraClsMode camera,False,True
	
	; Set the camera's viewport so it is the same size as our texture - so we can fit entire screen contents into texture
	CameraViewport camera,0,0,tex_sz,tex_sz

	; Update cubemap

	; do left view	
	SetCubeFace tex,0
	RotateEntity camera,0,90,0
	RenderWorld
	CopyRect 0,0,tex_sz,tex_sz,0,0,BackBuffer(),TextureBuffer(tex)
	
	; do forward view
	SetCubeFace tex,1
	RotateEntity camera,0,0,0
	RenderWorld
	CopyRect 0,0,tex_sz,tex_sz,0,0,BackBuffer(),TextureBuffer(tex)
	
	; do right view	
	SetCubeFace tex,2
	RotateEntity camera,0,-90,0
	RenderWorld
	CopyRect 0,0,tex_sz,tex_sz,0,0,BackBuffer(),TextureBuffer(tex)
	
	; do backward view
	SetCubeFace tex,3
	RotateEntity camera,0,180,0
	RenderWorld
	CopyRect 0,0,tex_sz,tex_sz,0,0,BackBuffer(),TextureBuffer(tex)
	
	; do up view
	SetCubeFace tex,4
	RotateEntity camera,-90,0,0
	RenderWorld
	CopyRect 0,0,tex_sz,tex_sz,0,0,BackBuffer(),TextureBuffer(tex)
	
	; do down view
	SetCubeFace tex,5
	RotateEntity camera,90,0,0
	RenderWorld
	CopyRect 0,0,tex_sz,tex_sz,0,0,BackBuffer(),TextureBuffer(tex)
	
	; Show entity again
	ShowEntity entity
	
	; Hide the cubemap camera
	HideEntity camera
	
End Function