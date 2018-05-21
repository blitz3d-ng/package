
void BBCALL bbGraphics3D( int w,int h,int d,int mode ){
	int flags=BBGraphics::GRAPHICS_3D;
	switch( mode ){
	case 0:flags|=(bb_env.debug && bbWindowed3D()) ? BBGraphics::GRAPHICS_WINDOWED : 0 ;break;
	case 1:break;
	case 2:flags|=BBGraphics::GRAPHICS_WINDOWED;break;
	case 3:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_SCALED;break;
	case 6:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_AUTOSUSPEND;break;
	case 7:flags|=BBGraphics::GRAPHICS_WINDOWED|BBGraphics::GRAPHICS_SCALED|BBGraphics::GRAPHICS_AUTOSUSPEND;break;
	default:RTEX( "Illegal Graphics3D mode" );
	}
	graphics( w,h,d,flags );
	extern void blitz3d_open( BBScene *scene );
	blitz3d_open( ((B3DGraphics*)((gxGraphics*)gx_graphics))->createScene( 0 ) );
}
