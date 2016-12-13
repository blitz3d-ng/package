Global FXCamera,CubeLightMapSize,CubeLightMapTexture,WaterMapSize,WaterMapTexture,waterdirection
Global camera,ball,level,campitch#,camyaw#,mvx#,mvy#,mvz#,DummyTexure,water,r#
Global turn,sprite,underwater,tab,player,firesprite,detail,floorpivot
Global fpsindex#,fpstime#,fpsfold_millisecs#,fpsfps#,wdb,wasunderwater,oldpicked
Global lcount=0
Dim firelight(8)

CubeLightMapSize = 256 ; the resolution of the dynamic lightmap *not used in this demo*
WaterMapSize = 256 ; the resolution of the water

Const C_level=1,C_player=2,C_floorpivot=3

Type layer
	Field mesh
	Field count
	Field spritebank ; pointer to a type just for holding a list of sprites
	Field emptybank  ; points to bank positions that are empty for filling
End Type

Type sprite
	Field x#,y#,z#,s#,a#
	Field vx#,vy#,vz#,vs#,va#
	Field r,g,b,vr#,vg#,vb#
	Field gravity#
	Field verti[3]
	Field surf,mesh,spawntype
	Field life
End Type

Type spawn
	Field time,spawntime,spawntype
	Field x#,y#,z#
End Type

Type deletelist
	Field ent,x#,y#,z#
End Type

Type bumptexture
	Field filename$
End Type

;----------------------------------------------------------------------------------

Global info1$="Cube mapped water effect"
Global info2$="Mouse to look around, W/A/S/Z to move"

Include "../start.bb"

HidePointer
AmbientLight 12,12,12
player=CreatePivot()
floorpivot=CreatePivot()
camera=CreateCamera(player)
CameraClsMode camera,False,True

MoveEntity camera,0,1.2,0
PositionEntity player,-52,8,353
CameraFogMode camera,1
CameraFogColor camera,100,200,255

FXCamera=CreateCamera()
CameraClsMode FXCamera,False,True
CameraProjMode FXCamera,0
CameraViewport FXCamera,0,0,WaterMapSize,WaterMapSize

;SET UP FIRE
Global fire=LoadTexture("level/fire.png",2+48)
Global firemesh=CreateLayer()
EntityTexture firemesh,fire
firesprite=LoadSprite("level/glow.png",2)
ScaleSprite firesprite,4.5,10
HideEntity firesprite


;SET UP LEVEL
level = LoadAnimMesh("level/test.b3d")
RecurseSeek(level)

;GRAB lights from scene and create some blitz lights, torches etc...
For d.deletelist=Each deletelist
	If lcount<8
		s.spawn=New spawn
		s\spawntime=2
		s\spawntype=0
		s\time=s\spawntime
		s\x=EntityX(d\ent,1)
		s\y=EntityY(d\ent,1)
		s\z=EntityZ(d\ent,1)	
		firelight(lcount) = CreateLight(2)
		PositionEntity firelight(lcount),EntityX(d\ent),EntityY(d\ent),EntityZ(d\ent)
		newsprite=CopyEntity(firesprite)
		PositionEntity newsprite,EntityX(d\ent),EntityY(d\ent)+1.5,EntityZ(d\ent)
	EndIf
	
	lcount=lcount+1
	
	FreeEntity d\ent
	Delete d
Next

; SET UP WATER - load a reference mesh 

water=LoadMesh("level/water.3ds")
MoveEntity water,0,-2,0
WaterMapTexture=CreateTexture(WaterMapSize,WaterMapSize,128+256+48)
EntityTexture water,WaterMapTexture
EntityColor water,100,200,255
EntityColor water,512,512,512
EntityAlpha water,0.7
EntityFX water,1
AddEntity(water)

Type Vertices
	Field x#
	Field y#
	Field z#
End Type
surf=GetSurface(water,1)
Dim Vertex.Vertices(CountVertices(surf))
For i=0 To CountVertices(surf)-1
	Vertex(i) = New Vertices
	Vertex(i)\x#=VertexX#(surf,i)
	Vertex(i)\y#=VertexY#(surf,i)
	Vertex(i)\z#=VertexZ#(surf,i)
Next


;FIRE
wnospawn=1
If wnospawn=0
s.spawn=New spawn
s\spawntime=2
s\spawntype=1
s\time=s\spawntime
s\x=-9
s\y=79
s\z=-305	

s.spawn=New spawn
s\spawntime=2
s\spawntype=1
s\time=s\spawntime
s\x=9
s\y=79
s\z=-305
EndIf
;----------------------------------------------------------------------------------

Collisions c_player,c_level,2,2
EntityType player,c_player
EntityType floorpivot,c_player

EntityType level,c_level,1
EntityRadius player,4.5,6
EntityRadius floorpivot,4
;----------------------------------------------------------------------------------

mainlight=CreateLight()
RotateEntity mainlight,45,45,45
LightColor mainlight,128,128,128

RotateEntity player,0,180,0
MoveMouse GraphicsWidth()/2,GraphicsHeight()/2
VWait : Flip

;MAINLOOP

While Not KeyHit(1)
	db=1-db
	FreeLook()
	RenderWater()
	UpdateNormals water		
	Spawn()
	RenderSprites()
	
	
	glow#=Cos(MilliSecs())
	lum#=(Abs(glow*Sin(glow))*255)*0.8
	For i=0 To 6
		LightColor firelight(i),220+lum,160+lum/2,100+lum/3
		LightRange firelight(i),((lum*1.8)+32)
	Next
	UpdateWorld
	RenderWorld
	
	;Text 0,0,fps()
	Flip
Wend
End

;----------------------------------------------------------------------------------

Function Spawn()
	For s.spawn=Each spawn
		s\time=s\time-1
		If s\time<0
			s\time=s\spawntime
			If s\spawntype=0 AddFire(s\x,s\y,s\z)
			;If s\spawntype=1 AddWater(s\x,s\y,s\z)
		EndIf
	Next
End Function

;----------------------------------------------------------------------------------

Function AddFire(fx#=0,fy#=0,fz#=0)

	;re-use if we can
	temp=0
	For s.sprite=Each sprite
		If s\mesh=firemesh
			If s\life=0
				temp = Handle(s)
				;counter=counter+1
				Exit
			EndIf
		EndIf
	Next
	If temp=0 temp = AddSprite(firemesh)
		
	vec#=Rnd(360)
	xpos#=-Cos(vec)*0.9
	ypos#=0
	zpos#=-Sin(vec)*0.9
	scale#=2.3
	alpha#=0.9
	
	velx#=-xpos*0.02
	vely#=Rnd(0.2)+0.1
	velz#=-zpos*0.02
	
	scalespd#=-0.08
	alphaspd#=-0.015
	gravity#=-0.002

	r=255
	g=255
	b=255
	
	vr#=-8
	vg#=-8
	vb#=-8

	
	life=80
	UpdateSprite temp,life,xpos+fx,ypos+fy,zpos+fz,velx,vely,velz,scale,scalespd,alpha,alphaspd,r,g,b,vr#,vg#,vb#,gravity#

End Function


;--------------------------------------------------------------------------------------------

Function RenderSprites()
	For s.sprite=Each sprite
		If s\life>0
			s\life=s\life-1			
			s\x=s\x+s\vx
			s\y=s\y+s\vy
			s\z=s\z+s\vz
			s\s=s\s+s\vs
			s\a=s\a+s\va
			s\r=s\r+s\vr
			s\g=s\g+s\vg
			s\b=s\b+s\vb
			s\vy=s\vy+s\gravity#
			
			TFormPoint s\x,s\y,s\z,0,camera
			If TFormedZ#()>0
				TFormVector -s\s,-s\s,0,camera,0 
				VertexCoords s\surf,s\verti[0],s\x+TFormedX#(),s\y+TFormedY#(),s\z+TFormedZ#()
				TFormVector s\s,-s\s,0,camera,0 
				VertexCoords s\surf,s\verti[1],s\x+TFormedX#(),s\y+TFormedY#(),s\z+TFormedZ#()
				TFormVector -s\s,s\s,0,camera,0 
				VertexCoords s\surf,s\verti[2],s\x+TFormedX#(),s\y+TFormedY#(),s\z+TFormedZ#()
				TFormVector s\s,s\s,0,camera,0 
				VertexCoords s\surf,s\verti[3],s\x+TFormedX#(),s\y+TFormedY#(),s\z+TFormedZ#()
				VertexColor s\surf,s\verti[0],s\r,s\g,s\b,s\a
				VertexColor s\surf,s\verti[1],s\r,s\g,s\b,s\a
				VertexColor s\surf,s\verti[2],s\r,s\g,s\b,s\a
				VertexColor s\surf,s\verti[3],s\r,s\g,s\b,s\a
			EndIf
			
		EndIf
	Next
End Function

;--------------------------------------------------------------------------------------------
Function CreateLayer()
	l.layer=New layer	
	l\mesh=CreateMesh() : surf=CreateSurface(l\mesh)
	l\spritebank=CreateBank()
	NameEntity l\mesh,Handle(l)
	EntityColor l\mesh,255,255,255
	EntityFX l\mesh,1+2
	EntityBlend l\mesh,3
	Return l\mesh
End Function
;--------------------------------------------------------------------------------------------
Function AddSprite(mesh)
	l.layer=Object.layer(EntityName(mesh))
	surf=GetSurface(l\mesh,1)
	l\count=l\count+1
	ResizeBank l\spritebank,l\count*4
	;create a new sprite
	s.sprite=New sprite
	s\verti[0]=AddVertex(surf,0,0,0,0,1) ;topleft
	s\verti[1]=AddVertex(surf,0,0,0,1,1) ;topright
	s\verti[2]=AddVertex(surf,0,0,0,0,0) ;bottomleft
	s\verti[3]=AddVertex(surf,0,0,0,1,0) ;bottomright
	AddTriangle(surf,s\verti[1],s\verti[2],s\verti[3])
	AddTriangle(surf,s\verti[2],s\verti[1],s\verti[0])
	s\s=10 : s\a=1
	s\r=255 : s\g=255 : s\b=255
	s\x=0 : s\y=0 : s\z=0
	s\surf=surf
	s\mesh=l\mesh
	;put the new sprite into the bank
	PokeInt l\spritebank,(l\count*4)-4,Handle(s)
	Return Handle(s)
End Function
;--------------------------------------------------------------------------------------------
Function UpdateSprite(sprite,life,x#,y#,z#,vx#,vy#,vz#,scale#,scalevel#,alpha#,alphavel#,r,g,b,vr#,vg#,vb#,gravity#)
	s.sprite = Object.sprite( sprite )
	s\x=x
	s\y=y
	s\z=z

	s\vx=vx
	s\vy=vy
	s\vz=vz	
	s\gravity#=gravity#
	s\s=scale
	s\vs=scalevel
	s\a=alpha
	s\va=alphavel

	s\r=r : s\g=g : s\b=b
	s\vr=vr : s\vg=vg : s\vb=vb

	
	s\life=life
End Function

;--------------------------------------------------------------------------------------------
Function fps()
	fpsindex=fpsindex+1
	fpstime=fpstime+MilliSecs()-fpsfold_millisecs
	If fpstime=>1000
		fpsfps=fpsindex
		fpstime=0
		fpsindex=0
	EndIf
	fpsfold_millisecs=MilliSecs()
	Return fpsfps
End Function
;--------------------------------------------------------------------------------------------
Function TransTex(texture,angle#,scale#=1)
	ScaleTexture texture,scale,scale
	RotateTexture texture,angle#
	x#=Cos(angle)/scale/2
	y#=Sin(angle)/scale/2
	PositionTexture texture,(x-.5)-y,(y-.5)+x
End Function

;----------------------------------------------------------------------------------

Function RenderWater()

	HideEntities()
	CameraProjMode camera,0
	CameraProjMode FXCamera,1
	
	wdb=1-wdb
		
	
	;flip water if under it
				
	If underwater=0	
		If waterdirection=-1
			FlipMesh water
			EntityAlpha water,.7
			EntityColor water,255,255,255
		EndIf
		waterdirection=1
		
		PositionEntity FXCamera,EntityX(player),EntityY(water)-2,EntityZ(player)

		
		
		If wdb
			;do left view	
			SetCubeFace WaterMapTexture,0
			RotateEntity FXCamera,0,90,0
			RenderWorld
			CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
			;do forward view
			SetCubeFace WaterMapTexture,1
			RotateEntity FXCamera,0,0,0
			RenderWorld
			CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
		Else
			;do right view	
			SetCubeFace WaterMapTexture,2
			RotateEntity FXCamera,0,-90,0
			RenderWorld
			CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
			;do backward view
			SetCubeFace WaterMapTexture,3
			RotateEntity FXCamera,0,180,0
			RenderWorld
			CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
		EndIf
		;do up view
		SetCubeFace WaterMapTexture,4
		RotateEntity FXCamera,-90,0,0
		RenderWorld
		CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
	
	Else
		If waterdirection=1
			FlipMesh water
			EntityAlpha water,1
			EntityColor water,155,200,255

		EndIf	
		waterdirection=-1
		
		PositionEntity FXCamera,-EntityX(player),EntityY(player)-2,EntityZ(player)
		
		
		If wdb
			;do left view	
			SetCubeFace WaterMapTexture,0
			RotateEntity FXCamera,0,-90,180
			RenderWorld
			CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
			;do forward view
			SetCubeFace WaterMapTexture,1
			RotateEntity FXCamera,0,0,180
			RenderWorld
			CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
		Else
			;do right view	
			SetCubeFace WaterMapTexture,2
			RotateEntity FXCamera,0,90,180
			RenderWorld
			CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
			;do backward view
			SetCubeFace WaterMapTexture,3
			RotateEntity FXCamera,0,180,180
			RenderWorld
			CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
		EndIf
		
		;do down view
		SetCubeFace WaterMapTexture,5
		RotateEntity FXCamera,-90,0,180
		RenderWorld
		CopyRect 0,0,WaterMapSize,WaterMapSize,0,0,BackBuffer(),TextureBuffer(WaterMapTexture)
	EndIf



	CameraProjMode FXCamera,0
	CameraProjMode camera,1
	ShowEntities()


	s=GetSurface(water,1)
	For i=0 To CountVertices(s)-1
		Freq#=MilliSecs()/4
		Vertex(i)\y#=Sin(freq+Vertex(i)\x#*500+Vertex(i)\z#*300)*1.2
		VertexCoords s,i,Vertex(i)\x#,-Vertex(i)\y#,Vertex(i)\z#
	Next

	If EntityY(camera,1)<-2 underwater=1 Else underwater=0

	If underwater
		CameraFogColor camera,50,100,155
		t#=MilliSecs()/6
		ScaleEntity camera,1+Cos(t)*0.04,1+Sin(t)*0.04,1+Cos(t)*0.02
		CameraFogRange camera,0,200+Sin(MilliSecs()*0.05)*60
		TurnEntity player,0,0,Sin(t)*2
	Else
		CameraFogColor camera,100,200,255
		ScaleEntity camera,1,1,1
		CameraFogRange camera,100,1000
	EndIf
	
End Function

;----------------------------------------------------------------------------------

Type entity
	Field ent
End Type

Type level
	Field ent
End Type

Type cubelight
	Field ent,r,g,b,radius
End Type

;----------------------------------------------------------------------------------
Function AddEntity(ent)
	e.entity=New entity
	e\ent=ent
End Function

;----------------------------------------------------------------------------------

Function HideEntities()
	For e.entity=Each entity
		HideEntity e\ent
	Next
End Function
;----------------------------------------------------------------------------------
Function ShowEntities()
	For e.entity=Each entity
		ShowEntity e\ent
	Next
End Function
;----------------------------------------------------------------------------------
Function AddLevel(ent)
	e.level=New level
	e\ent=ent
End Function
;----------------------------------------------------------------------------------

Function HideLevel()
	For e.level=Each level
		HideEntity e\ent
	Next
End Function
;----------------------------------------------------------------------------------

Function ShowLevel()
	For e.level=Each level
		ShowEntity e\ent
	Next
End Function

;----------------------------------------------------------------------------------


Function freelook()

	mxspd#=MouseXSpeed()*0.25
	myspd#=MouseYSpeed()*0.25
	
	If underwater
		mxspd=mxspd*0.6
		myspd=myspd*0.6
	EndIf
	
		
	MoveMouse GraphicsWidth()/2,GraphicsHeight()/2

	campitch=campitch+myspd
	If campitch<-85 Then campitch=-85
	If campitch>85 Then campitch=85
	RotateEntity player,campitch,EntityYaw(player)-mxspd,0
	
	If KeyDown(203) Then mvx=mvx-.3
	If KeyDown(205) Then mvx=mvx+.3
	If KeyDown(200) Then mvz=mvz+.3
	If KeyDown(208) Then mvz=mvz-.3
	
	If KeyDown(30) Then mvx=mvx-.3
	If KeyDown(32) Then mvx=mvx+.3
	If KeyDown(17) Then mvz=mvz+.3
	If KeyDown(31) Then mvz=mvz-.3
	
	
	PositionEntity floorpivot,EntityX(player),EntityY(player)-8,EntityZ(player)
	
	If underwater
	
		mvx=mvx/1.2
		mvy=mvy-0.005
		mvz=mvz/1.2
		wasunderwater=1
	
	Else
		
		mvy=mvy-0.2
		If EntityCollided(floorpivot,c_level)
			mvy=mvy+0.2
		EndIf
		
		If wasunderwater
			wasunderwater=0
			mvy=4
		EndIf
		
	EndIf
		
	mvx=mvx/1.2
	mvy=mvy/1.2
	mvz=mvz/1.2
	
	MoveEntity player,mvx,0,mvz
	TranslateEntity player,0,mvy,0
	
	If EntityY(player)>60 Then PositionEntity player,EntityX(player),60,EntityZ(player)
	
End Function

Function RecurseSeek(ent)
	tab=tab+4
	For i=1 To CountChildren(ent)	
		child=GetChild(ent,i)
		name$=Lower(EntityName(child))
		If Instr(name$,"fire")
			d.deletelist=New deletelist
			d\ent=child
		EndIf			
	Next
	tab=tab-4
End Function