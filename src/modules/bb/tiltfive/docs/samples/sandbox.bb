Graphics3D 1280,720,32,2
SetBuffer BackBuffer()

SetFont LoadFont("courier new", 22)

T5_Initialize

board=CreateCube()
FitMesh board,T5_GameboardX(),T5_GameboardY(),T5_GameboardZ(),T5_GameboardWidth(),T5_GameboardHeight(),T5_GameboardDepth()

CreateLight

cam=CreateCamera()
PositionEntity cam,2,2,-2

PointEntity cam,board

While Not KeyDown(1)
	RenderWorld

	T5_Poll

	Text 0,FontHeight()*0,"T5_ServiceVersion: "+T5_ServiceVersion()
	Text 0,FontHeight()*1,"T5_UIRequestingAttention: "+T5_UIRequestingAttention()
	Text 0,FontHeight()*2,"T5_GameboardX: "+T5_GameboardX()
	Text 0,FontHeight()*3,"T5_GameboardY: "+T5_GameboardY()
	Text 0,FontHeight()*4,"T5_GameboardZ: "+T5_GameboardZ()
	Text 0,FontHeight()*5,"T5_GameboardWidth:  "+T5_GameboardWidth()
	Text 0,FontHeight()*6,"T5_GameboardHeight: "+T5_GameboardHeight()
	Text 0,FontHeight()*7,"T5_GameboardDepth:  "+T5_GameboardDepth()

	Text 0,FontHeight()*9,"T5_CountGlasses: "+T5_CountGlasses()
	For i = 0 To T5_CountGlasses()-1
		Text 0,FontHeight()*(10+i*3),"  T5_GlassesIdentifier:   "+T5_GlassesIdentifier(i)
		Text 0,FontHeight()*(11+i*3),"  T5_GlassesFriendlyName: "+T5_GlassesFriendlyName(i)
		Text 0,FontHeight()*(12+i*3),"  T5_GlassesIPD:          "+T5_GlassesIPD(i)
	Next

	Flip
Wend
