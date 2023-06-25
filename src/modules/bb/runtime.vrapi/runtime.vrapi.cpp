#include "../../../config.h"
#include "runtime.vrapi.h"
#include <bb/graphics.gl/graphics.gl.h>
#include <bb/blitz3d.gl/blitz3d.gl.h>
#include <bb/runtime/runtime.h>
#include <bb/graphics.gl/egl.h>
#include <bb/blitz3d/commands.h>
#include <bb/blitz3d/geom.h>
#include <bb/system.ndk/system.ndk.h>
#include <bb/blitz/app.h>


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/prctl.h> // for prctl( PR_SET_NAME )
#include <android_native_app_glue.h>

#include <VrApi.h>
#include <VrApi_Helpers.h>
#include <VrApi_SystemUtils.h>
#include <VrApi_Input.h>

#include "ext.h"

ovrJava java;

static const float dtor=0.0174532925199432957692369076848861f;

static double GetTimeInSeconds() {
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (now.tv_sec * 1e9 + now.tv_nsec) * 0.000000001;
}


class VRAPIEyeCanvas : public GLCanvas{
protected:
public:
	int Multisamples;
	int TextureSwapChainLength;
	int TextureSwapChainIndex,LastTextureSwapChainIndex;
	bool UseMultiview;
	ovrTextureSwapChain* ColorTextureSwapChain;
	uint32_t* DepthBuffers;
	uint32_t* FrameBuffers;

	VRAPIEyeCanvas(ContextResources *res):GLCanvas(res,0){
		clear();
	}

	int getDepth()const{return 32;}

	void unset(){
		const GLenum depthAttachment[1]={ GL_DEPTH_ATTACHMENT };
		GL( glInvalidateFramebuffer( GL_DRAW_FRAMEBUFFER,1,depthAttachment ) );
		LastTextureSwapChainIndex=TextureSwapChainIndex;
		TextureSwapChainIndex = (TextureSwapChainIndex + 1) % TextureSwapChainLength;
	}
	void set(){
		GL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER,framebufferId() ) );
	}

	void bind()const{}
	unsigned int framebufferId(){ return FrameBuffers[TextureSwapChainIndex]; }

	void clear(){
		width=height=0;
		Multisamples = 0;
		TextureSwapChainLength = 0;
		TextureSwapChainIndex = 0;
		UseMultiview = false;
		ColorTextureSwapChain = NULL;
		DepthBuffers = NULL;
		FrameBuffers = NULL;
	}

	bool init( const bool useMultiview,const GLenum colorFormat,const int w,const int h,const int multisamples ){
		width=w;height=h;

		PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT=(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)eglGetProcAddress( "glRenderbufferStorageMultisampleEXT" );
		PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glFramebufferTexture2DMultisampleEXT=(PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)eglGetProcAddress( "glFramebufferTexture2DMultisampleEXT" );

	    PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC glFramebufferTextureMultiviewOVR =
	        (PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC)eglGetProcAddress(
	            "glFramebufferTextureMultiviewOVR");
	    PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC glFramebufferTextureMultisampleMultiviewOVR =
	        (PFNGLFRAMEBUFFERTEXTUREMULTISAMPLEMULTIVIEWOVRPROC)eglGetProcAddress(
	            "glFramebufferTextureMultisampleMultiviewOVR");

	    Multisamples = multisamples;
	    UseMultiview =
	        (useMultiview && (glFramebufferTextureMultiviewOVR != NULL)) ? true : false;

	    ColorTextureSwapChain = vrapi_CreateTextureSwapChain3(
	        UseMultiview ? VRAPI_TEXTURE_TYPE_2D_ARRAY : VRAPI_TEXTURE_TYPE_2D,
	        colorFormat,
	        width,
	        height,
	        1,
	        3);
	    TextureSwapChainLength =
	        vrapi_GetTextureSwapChainLength(ColorTextureSwapChain);
	    DepthBuffers =
	        (GLuint*)malloc(TextureSwapChainLength * sizeof(GLuint));
	    FrameBuffers =
	        (GLuint*)malloc(TextureSwapChainLength * sizeof(GLuint));

	    LOGD("        UseMultiview = %d", UseMultiview);

	    for (int i = 0; i < TextureSwapChainLength; i++) {
	        // Create the color buffer texture.
	        const GLuint colorTexture =
	            vrapi_GetTextureSwapChainHandle(ColorTextureSwapChain, i);
	        GLenum colorTextureTarget = UseMultiview ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
	        GL(glBindTexture(colorTextureTarget, colorTexture));
	        GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	        GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	        GLfloat borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	        GL(glTexParameterfv(colorTextureTarget, GL_TEXTURE_BORDER_COLOR, borderColor));
	        GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	        GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	        GL(glBindTexture(colorTextureTarget, 0));

	        if (UseMultiview) {
	            // Create the depth buffer texture.
	            GL(glGenTextures(1, &DepthBuffers[i]));
	            GL(glBindTexture(GL_TEXTURE_2D_ARRAY, DepthBuffers[i]));
	            GL(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT24, width, height, 2));
	            GL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));

	            // Create the frame buffer.
	            GL(glGenFramebuffers(1, &FrameBuffers[i]));
	            GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBuffers[i]));
	            if (multisamples > 1 && (glFramebufferTextureMultisampleMultiviewOVR != NULL)) {
	                GL(glFramebufferTextureMultisampleMultiviewOVR(
	                    GL_DRAW_FRAMEBUFFER,
	                    GL_DEPTH_ATTACHMENT,
	                    DepthBuffers[i],
	                    0 /* level */,
	                    multisamples /* samples */,
	                    0 /* baseViewIndex */,
	                    2 /* numViews */));
	                GL(glFramebufferTextureMultisampleMultiviewOVR(
	                    GL_DRAW_FRAMEBUFFER,
	                    GL_COLOR_ATTACHMENT0,
	                    colorTexture,
	                    0 /* level */,
	                    multisamples /* samples */,
	                    0 /* baseViewIndex */,
	                    2 /* numViews */));
	            } else {
	                GL(glFramebufferTextureMultiviewOVR(
	                    GL_DRAW_FRAMEBUFFER,
	                    GL_DEPTH_ATTACHMENT,
	                    DepthBuffers[i],
	                    0 /* level */,
	                    0 /* baseViewIndex */,
	                    2 /* numViews */));
	                GL(glFramebufferTextureMultiviewOVR(
	                    GL_DRAW_FRAMEBUFFER,
	                    GL_COLOR_ATTACHMENT0,
	                    colorTexture,
	                    0 /* level */,
	                    0 /* baseViewIndex */,
	                    2 /* numViews */));
	            }

	            GL(GLenum renderFramebufferStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER));
	            GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	            if (renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
	                LOGD(
	                    "Incomplete frame buffer object: %s",
	                    bbGLFramebufferStatusString(renderFramebufferStatus));
	                return false;
	            }
	        } else {
	            if (multisamples > 1 && glRenderbufferStorageMultisampleEXT != NULL &&
	                glFramebufferTexture2DMultisampleEXT != NULL) {
	                // Create multisampled depth buffer.
	                GL(glGenRenderbuffers(1, &DepthBuffers[i]));
	                GL(glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffers[i]));
	                GL(glRenderbufferStorageMultisampleEXT(
	                    GL_RENDERBUFFER, multisamples, GL_DEPTH_COMPONENT24, width, height));
	                GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	                // Create the frame buffer.
	                // NOTE: glFramebufferTexture2DMultisampleEXT only works with GL_FRAMEBUFFER.
	                GL(glGenFramebuffers(1, &FrameBuffers[i]));
	                GL(glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffers[i]));
	                GL(glFramebufferTexture2DMultisampleEXT(
	                    GL_FRAMEBUFFER,
	                    GL_COLOR_ATTACHMENT0,
	                    GL_TEXTURE_2D,
	                    colorTexture,
	                    0,
	                    multisamples));
	                GL(glFramebufferRenderbuffer(
	                    GL_FRAMEBUFFER,
	                    GL_DEPTH_ATTACHMENT,
	                    GL_RENDERBUFFER,
	                    DepthBuffers[i]));
	                GL(GLenum renderFramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	                GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	                if (renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
	                    LOGD(
	                        "Incomplete frame buffer object: %s",
	                        bbGLFramebufferStatusString(renderFramebufferStatus));
	                    return false;
	                }
	            } else {
	                // Create depth buffer.
	                GL(glGenRenderbuffers(1, &DepthBuffers[i]));
	                GL(glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffers[i]));
	                GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height));
	                GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	                // Create the frame buffer.
	                GL(glGenFramebuffers(1, &FrameBuffers[i]));
	                GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBuffers[i]));
	                GL(glFramebufferRenderbuffer(
	                    GL_DRAW_FRAMEBUFFER,
	                    GL_DEPTH_ATTACHMENT,
	                    GL_RENDERBUFFER,
	                    DepthBuffers[i]));
	                GL(glFramebufferTexture2D(
	                    GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0));
	                GL(GLenum renderFramebufferStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER));
	                GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	                if (renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
	                    LOGD(
	                        "Incomplete frame buffer object: %s",
	                        bbGLFramebufferStatusString(renderFramebufferStatus));
	                    return false;
	                }
	            }
	        }
	    }

	    return true;
	}

	void destroy(){
		GL(glDeleteFramebuffers(TextureSwapChainLength, FrameBuffers));
		if( UseMultiview ){
			GL(glDeleteTextures(TextureSwapChainLength, DepthBuffers));
		}else{
			GL(glDeleteRenderbuffers(TextureSwapChainLength, DepthBuffers));
		}
		vrapi_DestroyTextureSwapChain(ColorTextureSwapChain);

		free(DepthBuffers);
		free(FrameBuffers);

		clear();
	}
};



Entity *HMDHead() {
	static Entity* pivot=0;
	if( !pivot ){
		pivot=bbCreatePivot( 0 );
	}
	return pivot;
}

Entity *HMDPivot() {
	static Entity* pivot=0;
	if( !pivot ){
		pivot=bbCreatePivot( HMDHead() );
	}
	return pivot;
}

Camera *HMDEye( int i ) {
	static Camera* eyes[2]={ 0,0 };
	if( !eyes[i] ){
		eyes[i]=bbCreateCamera( HMDPivot() )->getCamera();
	}
	return eyes[i];
}

Entity *HMDHands() {
	static Entity* pivot=0;
	if( !pivot ){
		pivot=bbCreatePivot( HMDHead() );
	}
	return pivot;
}

bb_int_t BBCALL bbHMDHead() {
	return (bb_int_t)HMDHead();
}

bb_int_t BBCALL bbHMDLeftEye() {
	return 0;
}

bb_int_t BBCALL bbHMDRightEye() {
	return 0;
}

bb_int_t BBCALL bbHMDPivot() {
	return (bb_int_t)HMDPivot();
}

class VRAPIDefaultCanvas : public GLDefaultCanvas{
protected:
public:
	VRAPIDefaultCanvas( ContextResources *res,unsigned framebuffer,int mode,int flags ):GLDefaultCanvas(res,framebuffer,mode,flags){}

	void getViewport( int *x,int *y,int *w,int *h )const{
		*x=0;*y=0;*w=getWidth();*h=getHeight();
	}
};

class VRAPIGraphics : public GLGraphics{
protected:
	int window_width,window_height,drawable_width,drawable_height;

	unsigned short gamma_red[256], gamma_green[256], gamma_blue[256];
public:
	VRAPIGraphics(){
		front_canvas=d_new VRAPIDefaultCanvas( &res,0,GL_FRONT,0 );
		back_canvas=d_new VRAPIDefaultCanvas( &res,0,GL_BACK,0 );

		drawable_width=window_width=1440;
		drawable_height=window_height=1584;

		for( int k=0;k<256;++k ) gamma_red[k]=gamma_green[k]=gamma_blue[k]=k;

		resize();
	}

	~VRAPIGraphics(){
		if( front_canvas ) delete front_canvas;
		if( back_canvas ) delete back_canvas;
		front_canvas=back_canvas=0;
	}

	void resize(){
	}

	// graphics
	void backup(){}
	bool restore(){ return true; }

	//MANIPULATORS
	void vwait(){}

	//SPECIAL!
	void copy( BBCanvas *dest,int dx,int dy,int dw,int dh,BBCanvas *src,int sx,int sy,int sw,int sh ){}

  //NEW! Gamma control!
	void setGamma( int r,int g,int b,float dr,float dg,float db ){
		gamma_red[r&255]=dr*257.0f;
		gamma_red[g&255]=dg*257.0f;
		gamma_red[b&255]=db*257.0f;
	}
	void getGamma( int r,int g,int b,float *dr,float *dg,float *db ){
		*dr=gamma_red[r&255];*dg=gamma_green[g&255];*db=gamma_blue[b&255];
	}

	void updateGamma( bool calibrate ){
	}

	//ACCESSORS
	int getWidth()const{ return drawable_width; }
	int getHeight()const{ return drawable_height; }
	int getLogicalWidth()const{ return window_width; };
	int getLogicalHeight()const{ return window_height; };
	int getDepth()const{ return 0; }
	float getDensity()const{ return (float)drawable_width/window_width; };
	int getScanLine()const{ return 0; }
	int getAvailVidmem()const{ return 0; }
	int getTotalVidmem()const{ return 0; }

	void moveMouse( int x,int y ){}
	void setPointerVisible( bool vis ){}
};


class VRAPIRuntime : public GLB3DGraphics, public BBRuntime, public BBContextDriver{
private:
	void setTitle( const char *title );

	void resize();

	android_app *app;
	int multi_view; // GL_OVR_multiview, GL_OVR_multiview2
	int EXT_texture_border_clamp; // GL_EXT_texture_border_clamp, GL_OES_texture_border_clamp

	ovrTracking2 tracking;
public:

	BBContextES Egl;
	ANativeWindow* NativeWindow;
	bool Resumed;
	ovrMobile* Ovr;
	long long FrameIndex;
	double DisplayTime;
	int SwapInterval;
	int CpuLevel;
	int GpuLevel;
	int MainThreadTid;
	int RenderThreadTid;
	bool UseMultiview;
	double startTime;

	std::vector<VRAPIEyeCanvas> eyes;

	void init(){
		((NDKSystemDriver*)bbSystemDriver)->setJNI( app->activity->clazz,java.Env );

		Egl.create(NULL);

		const char* exts=(const char*)glGetString( GL_EXTENSIONS );
		if( exts!=NULL ){
			multi_view = strstr(exts, "GL_OVR_multiview2") && strstr(exts, "GL_OVR_multiview_multisampled_render_to_texture");
			EXT_texture_border_clamp = strstr(exts, "GL_EXT_texture_border_clamp") || strstr(exts, "GL_OES_texture_border_clamp");
		}

		// loadingIcon();

		GL( glDisable(GL_FRAMEBUFFER_SRGB_EXT) );

		// UseMultiview &= glExtensions.multi_view;
		UseMultiview = false;

		LOGD("AppState UseMultiview : %d", UseMultiview);

		CpuLevel = 2;
		GpuLevel = 3;
		MainThreadTid = gettid();

		VRAPIGraphics *g=new VRAPIGraphics();
		g->init();
		graphics=g;

		// create eye buffers
		bool multiview=false;
		for ( int eye=0;eye<(multiview?1:VRAPI_FRAME_LAYER_EYE_MAX);eye++ ){
			eyes.push_back( VRAPIEyeCanvas(&g->res) );

			eyes[eye].init(
				multiview,
				GL_SRGB8_ALPHA8,
				vrapi_GetSystemPropertyInt( &java,VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_WIDTH),
				vrapi_GetSystemPropertyInt( &java,VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_HEIGHT),
				4
			);
		}

		app->userData = this;
		app->onAppCmd = app_handle_cmd;

		startTime = GetTimeInSeconds();
	}

	static void app_handle_cmd(struct android_app* app, int32_t cmd) {
		VRAPIRuntime* rt = (VRAPIRuntime*)app->userData;

		switch (cmd) {
		case APP_CMD_START:
			break;
		case APP_CMD_RESUME:
			rt->Resumed = true;
			break;
		case APP_CMD_PAUSE:
			rt->Resumed = false;
			break;
		case APP_CMD_STOP:
			break;
		case APP_CMD_DESTROY:
			rt->NativeWindow = NULL;
			break;
		case APP_CMD_INIT_WINDOW:
			rt->NativeWindow = app->window;
			break;
		case APP_CMD_TERM_WINDOW:
			rt->NativeWindow = NULL;
			break;
		}
	}


	void destroy(){
		for (int eye = 0; eye < eyes.size(); eye++) {
			eyes[eye].destroy();
		}

		Egl.destroy();
	}

	void ovrApp_HandleVrModeChanges() {
		if (Resumed != false && NativeWindow != NULL) {
			if (Ovr == NULL) {
				ovrModeParms parms = vrapi_DefaultModeParms(&java);
				// No need to reset the FLAG_FULLSCREEN window flag when using a View
				parms.Flags &= ~VRAPI_MODE_FLAG_RESET_WINDOW_FULLSCREEN;

				parms.Flags |= VRAPI_MODE_FLAG_FRONT_BUFFER_SRGB;
				parms.Flags |= VRAPI_MODE_FLAG_NATIVE_WINDOW;
				parms.Display = (size_t)Egl.display;
				parms.WindowSurface = (size_t)NativeWindow;
				parms.ShareContext = (size_t)Egl.context;

				Ovr = vrapi_EnterVrMode(&parms);

				// If entering VR mode failed then the ANativeWindow was not valid.
				if (Ovr == NULL) {
					LOGD("%s", "Invalid ANativeWindow!");
					NativeWindow = NULL;
				}

				if ( Ovr ){
					vrapi_SetClockLevels( Ovr,CpuLevel,GpuLevel );
					vrapi_SetPerfThread( Ovr,VRAPI_PERF_THREAD_TYPE_MAIN,MainThreadTid );
					vrapi_SetPerfThread( Ovr,VRAPI_PERF_THREAD_TYPE_RENDERER,RenderThreadTid );
					vrapi_SetTrackingSpace( Ovr,VRAPI_TRACKING_SPACE_LOCAL_FLOOR );
					vrapi_SetExtraLatencyMode( Ovr,VRAPI_EXTRA_LATENCY_MODE_ON );
					vrapi_SetPropertyInt( &java,VRAPI_DYNAMIC_FOVEATION_ENABLED,true );
					vrapi_SetDisplayRefreshRate( Ovr,120.0 );
					// ovrHmdColorDesc color_desc={ VRAPI_COLORSPACE_P3 };
					// vrapi_SetClientColorDesc( Ovr,&color_desc );
				}
			}
		} else {
			if ( Ovr ) {
				vrapi_LeaveVrMode(Ovr);
				Ovr=0;
			}
		}
	}

	void ovrApp_HandleVrApiEvents() {
		ovrEventDataBuffer eventDataBuffer = {};

		// Poll for VrApi events
		for (;;) {
			ovrEventHeader* eventHeader = (ovrEventHeader*)(&eventDataBuffer);
			ovrResult res = vrapi_PollEvent(eventHeader);
			if (res != ovrSuccess) {
				break;
			}

			switch (eventHeader->EventType) {
			case VRAPI_EVENT_DATA_LOST:
				break;
			case VRAPI_EVENT_VISIBILITY_GAINED:
				break;
			case VRAPI_EVENT_VISIBILITY_LOST:
				break;
			case VRAPI_EVENT_FOCUS_GAINED:
				HMDHands()->setVisible( true );
				break;
			case VRAPI_EVENT_FOCUS_LOST:
				HMDHands()->setVisible( false );
				break;
			default:
				break;
			}
		}
	}


	VRAPIRuntime( android_app *app );
	~VRAPIRuntime();

	// context driver
	int numGraphicsDrivers(){
		return 0; // TODO
	}
	void graphicsDriverInfo( int driver,std::string *name,int *c ){
		// TODO
	}
	int numGraphicsModes( int driver ){
		// TODO
		return 0;
	}
	void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
		// TODO
	}
	void windowedModeInfo( int *c ){
		// TODO
	}

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags ){return graphics;}
	void closeGraphics( BBGraphics *graphics ){}
	bool graphicsLost(){return false;}

	void loadingIcon(){
		int frameFlags=0;
		frameFlags|=VRAPI_FRAME_FLAG_FLUSH;

		ovrLayerProjection2 blackLayer=vrapi_DefaultLayerBlackProjection2();
		blackLayer.Header.Flags|=VRAPI_FRAME_LAYER_FLAG_INHIBIT_SRGB_FRAMEBUFFER;

		ovrLayerLoadingIcon2 iconLayer=vrapi_DefaultLayerLoadingIcon2();
		iconLayer.Header.Flags|=VRAPI_FRAME_LAYER_FLAG_INHIBIT_SRGB_FRAMEBUFFER;

		const ovrLayerHeader2* layers[] = {
			&blackLayer.Header,
			&iconLayer.Header
		};

		ovrSubmitFrameDescription2 frameDesc={ 0 };
		frameDesc.Flags=frameFlags;
		frameDesc.SwapInterval=1;
		frameDesc.FrameIndex=FrameIndex;
		frameDesc.DisplayTime=DisplayTime;
		frameDesc.LayerCount=2;
		frameDesc.Layers=layers;

		vrapi_SubmitFrame2( Ovr,&frameDesc );

	}

	void updateTracking(){
		if( Ovr==NULL ){
			return;
		}

		// This is the only place the frame index is incremented, right before
		// calling vrapi_GetPredictedDisplayTime().
		FrameIndex++;

		// Get the HMD pose, predicted for the middle of the time period during which
		// the new eye images will be displayed. The number of frames predicted ahead
		// depends on the pipeline depth of the engine and the synthesis rate.
		// The better the prediction, the less black will be pulled in at the edges.
		const double predictedDisplayTime=vrapi_GetPredictedDisplayTime( Ovr,FrameIndex );
		tracking=vrapi_GetPredictedTracking2( Ovr,predictedDisplayTime );

		DisplayTime = predictedDisplayTime;

		double displayTimeInSeconds=predictedDisplayTime - startTime;

		for (uint32_t deviceIndex = 0;; deviceIndex++) {
			ovrInputCapabilityHeader curCaps;

			if (vrapi_EnumerateInputDevices(Ovr, deviceIndex, &curCaps) < 0) {
				// LOGD( "Input - No more devices!" );
				break; // no more devices
			}

			ovrResult result = ovrError_NotInitialized;
			switch (curCaps.Type) {
			case ovrControllerType_Hand: {
				// LOGD("VrHands - Hand connected, ID = %u", curCaps.DeviceID);

				ovrInputHandCapabilities handCapabilities;
				handCapabilities.Header = curCaps;

				// LOGD("VrHands - calling get device caps");
				result = vrapi_GetInputDeviceCapabilities(Ovr, &handCapabilities.Header);
				// LOGD("VrHands - post calling get device caps %d", result);

				struct Hand {
					ovrDeviceID device_id=0;
					Entity *root=0;
					std::vector<Model*> bones;
				};

				static Hand left_hand;
				static Hand right_hand;

				if (result == ovrSuccess) {
					ovrInputStateHand handInputState;
					handInputState.Header.ControllerType = curCaps.Type;

					ovrResult result = vrapi_GetCurrentInputState( Ovr,curCaps.DeviceID,&handInputState.Header );
					if (result == ovrSuccess) {
						ovrHandedness handedness = (handCapabilities.HandCapabilities & ovrHandCaps_LeftHand) != 0
							? VRAPI_HAND_LEFT
							: VRAPI_HAND_RIGHT;

						// LOGD("handedness = %u, left_hand=%u,right_hand=%u", handedness, left_hand.device_id,right_hand.device_id);

						Hand *hand=handedness==VRAPI_HAND_LEFT?&left_hand:&right_hand;
						if( hand->device_id==0 ){
							hand->device_id=curCaps.DeviceID;

							hand->root=bbCreatePivot( HMDHands() );
							// hand->root->setLocalPosition( Vector(0,0,2) );


							static MeshModel *bb=0;
							if( bb==0 ){
								Brush b;
								b.setColor( Vector( 1,0,0 ) );
								b.setAlpha( .75f );
								bb=MeshUtil::createSphere( b,16 );
								Transform t;
								t.m.i.x=.005f * 2.0f;
								t.m.j.y=.005f;
								t.m.k.z=.005f;
								bb->transform( t );
							}

							bb->clone()->setParent( hand->root );

							ovrHandSkeleton skeleton;
							skeleton.Header.Version = ovrHandVersion_1;
							if( vrapi_GetHandSkeleton( Ovr,handedness,&skeleton.Header)==ovrSuccess ){
								LOGD("VrHands - got a skeleton ... NumBones:%u", skeleton.NumBones);

								hand->bones.resize( skeleton.NumBones );
								for (uint32_t i = 0; i < skeleton.NumBones; ++i) {
									hand->bones[i]=bb->clone()->getModel();
								}

								for (uint32_t i = 0; i < skeleton.NumBones; ++i) {
									ovrPosef pose = skeleton.BonePoses[i];
									LOGD(
										"Posef{ Quatf{ %.6f, %.6f, %.6f, %.6f }, Vector3f{ %.6f, %.6f, %.6f } }, // bone=%u parent=%d",
										pose.Orientation.x, pose.Orientation.y, pose.Orientation.z, pose.Orientation.w,
										pose.Translation.x, pose.Translation.y, pose.Translation.z,
										i,
										(int)skeleton.BoneParentIndices[i]
									);

									int parent_idx=(int)skeleton.BoneParentIndices[i];

									Model *bone=hand->bones[i];
									bone->setParent( parent_idx==-1?hand->root:hand->bones[parent_idx] );
									bone->setLocalPosition( Vector( pose.Translation.x,pose.Translation.y,-pose.Translation.z ) );
									bone->setLocalRotation( Quat( pose.Orientation.w,Vector(pose.Orientation.x,pose.Orientation.y,-pose.Orientation.z ) ) );
								}
							} else {
								LOGD("%s", "VrHands - failed to get hand skeleton");
							}

							// {
							//     std::unique_ptr<ovrHandMesh> mesh(new ovrHandMesh());
							//     if (mesh) {
							//         mesh->Header.Version = ovrHandVersion_1;
							//         if (vrapi_GetHandMesh( Ovr,handedness,&(mesh.get()->Header) ) !=
							//             ovrSuccess) {
							//             LOGD("VrHands - failed to get hand mesh");
							//         } else {
							//             handDevice->InitFromSkeletonAndMesh(*this, &skeleton, mesh.get());
							//         }
							//     }
							// }

						}else{
							ovrPosef HandPose;
							ovrTracking Tracking;
							ovrInputStateHand InputStateHand;
							ovrHandPose RealHandPose;
							ovrResult result=vrapi_GetInputTrackingState( Ovr,hand->device_id,displayTimeInSeconds,&Tracking);
							if (result != ovrSuccess) {
									continue;
							} else {
									HandPose = Tracking.HeadPose.Pose;
							}

							ovrResult r = ovrSuccess;
							InputStateHand.Header.ControllerType = ovrControllerType_Hand;
							r = vrapi_GetCurrentInputState( Ovr,hand->device_id, &InputStateHand.Header);
							if (r != ovrSuccess) {
								LOGD( "%s", "VrHands - failed to get hand input state.");
								continue;
							}

							RealHandPose.Header.Version = ovrHandVersion_1;
							r = vrapi_GetHandPose( Ovr,hand->device_id, displayTimeInSeconds, &(RealHandPose.Header));
							if (r != ovrSuccess) {
								LOGD( "%s", "VrHands - failed to get hand pose");
								continue;
							} else {
								// SampleConfiguration.HandScaleFactor = RealHandPose.HandScale;
								// {
								// 		VRMenuObject* scaleDisplay = IsLeftHand() ? HandScaleDisplayL : HandScaleDisplayR;
								// 		std::ostringstream ss;
								// 		ss << (IsLeftHand() ? "size L " : "size R ") << RealHandPose.HandScale;
								// 		scaleDisplay->SetText(ss.str().c_str());
								// }
								//
								// /// Get the root pose from the API
								// HandPose = RealHandPose.RootPose;
								// /// Pointer poses
								// PointerPose = InputStateHand.PointerPose;
								// /// update based on hand pose
								// HandModel.Update(RealHandPose);
								// UpdateSkeleton(HandPose);

								// LOGD("update!!!!");

								// RootPose

								ovrPosef pose=RealHandPose.RootPose;

								// LOGD( "hand=%f,%f,%f",pose.Translation.x,pose.Translation.y,-pose.Translation.z );
								// LOGD( "%f",RealHandPose.HandScale );

								hand->root->setLocalPosition( Vector( pose.Translation.x,pose.Translation.y,-pose.Translation.z ) );
								hand->root->setLocalRotation( Quat( pose.Orientation.w,Vector(pose.Orientation.x,pose.Orientation.y,-pose.Orientation.z ) ) );

								for( size_t i=0;i<hand->bones.size();i++ ){
									ovrQuatf rot=RealHandPose.BoneRotations[i];
									hand->bones[i]->setLocalRotation( Quat( rot.w,Vector(rot.x,rot.y,-rot.z ) ) );
								}
							}

						}
					}
	      }
					break;
				}
				default: {
					// LOGD("Unknown device connected!");
				}
			}
		}

		// Render eye images and setup the primary layer using ovrTracking2.
		ovrMatrix4f eyeViewMatrixTransposed[2];
		eyeViewMatrixTransposed[0] = ovrMatrix4f_Transpose(&tracking.Eye[0].ViewMatrix);
		eyeViewMatrixTransposed[1] = ovrMatrix4f_Transpose(&tracking.Eye[1].ViewMatrix);

		ovrMatrix4f projectionMatrixTransposed[2];
		projectionMatrixTransposed[0] = ovrMatrix4f_Transpose(&tracking.Eye[0].ProjectionMatrix);
		projectionMatrixTransposed[1] = ovrMatrix4f_Transpose(&tracking.Eye[1].ProjectionMatrix);

		ovrVector3f headPosition=tracking.HeadPose.Pose.Position;
		ovrQuatf headOrientation=tracking.HeadPose.Pose.Orientation;

		// Render the eye images.
		for (int eye = 0; eye < eyes.size(); eye++) {
			// NOTE: In the non-mv case, latency can be further reduced by updating the sensor
			// prediction for each eye (updates orientation, not position)

			ovrMatrix4f view=ovrMatrix4f_Inverse( &eyeViewMatrixTransposed[eye] );

			Quat quat=matrixQuat(Matrix(
				Vector( view.M[0][0],view.M[1][0],view.M[2][0] ),
				Vector( view.M[0][1],view.M[1][1],view.M[2][1] ),
				Vector( view.M[0][2],view.M[1][2],view.M[2][2] )
			));
			quat.v.z*=-1;

			Transform tform(
				Matrix( quat ),
				Vector( view.M[3][0],view.M[3][1],-view.M[3][2] )
			);

			float left,right,up,down;
			ovrMatrix4f_ExtractFov(&tracking.Eye[eye].ProjectionMatrix, &left,&right,&up,&down);

			Camera *c=HMDEye( eye );
			c->setLocalTform( tform );
			c->setRange( 0.01,1000 );
			c->setFOV( left*dtor,right*dtor,up*dtor,down*dtor );
			c->setCanvas( &eyes[eye] );
			c->setViewport( 0,0,eyes[eye].getWidth(),eyes[eye].getHeight() );
		}
	}


	void flip( bool vwait ){
		if( Ovr==NULL ){
			return;
		}

		ovrLayerProjection2 worldLayer = vrapi_DefaultLayerProjection2();
		worldLayer.HeadPose = tracking.HeadPose;
		for (int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++) {
			VRAPIEyeCanvas *fb=&eyes[eyes.size()==1?0:eye];
			worldLayer.Textures[eye].ColorSwapChain = fb->ColorTextureSwapChain;
			worldLayer.Textures[eye].SwapChainIndex = fb->LastTextureSwapChainIndex;
			worldLayer.Textures[eye].TexCoordsFromTanAngles = ovrMatrix4f_TanAngleMatrixFromProjection(&tracking.Eye[eye].ProjectionMatrix);
		}
		worldLayer.Header.Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;

		const ovrLayerHeader2* layers[]={ &worldLayer.Header };

		ovrSubmitFrameDescription2 frameDesc = {0};
		frameDesc.Flags = 0;
		frameDesc.SwapInterval = SwapInterval;
		frameDesc.FrameIndex = FrameIndex;
		frameDesc.DisplayTime = DisplayTime;
		frameDesc.LayerCount = 1;
		frameDesc.Layers = layers;

		// Hand over the eye images to the time warp.
		vrapi_SubmitFrame2(Ovr, &frameDesc);
	}

	// runtime
	void asyncStop(){}
	void asyncRun(){}
	void asyncEnd(){
		ANativeActivity_finish( app->activity );
	}

	bool idle(){
		// Read all pending events.
		for (;;) {
			int events;
			struct android_poll_source* source;
			const int timeoutMilliseconds=(Ovr==NULL&&app->destroyRequested==0)?-1:0;
			if( ALooper_pollAll( timeoutMilliseconds,NULL,&events,(void**)&source )<0 ){
				break;
			}

			// Process this event.
			if( source!=NULL ){
				source->process(app, source);
			}

			ovrApp_HandleVrModeChanges( );
		}

		// We must read from the event queue with regular frequency.
		ovrApp_HandleVrApiEvents( );

		// TODO: update controller/hand data

		return app->destroyRequested==0;
	}

	void *window(){return 0;}

	void moveMouse( int x,int y ){}
	void setPointerVisible( bool vis ){}
};

VRAPIRuntime::VRAPIRuntime( android_app *app ):app(app){
	bbContextDriver=this;
	bbSceneDriver=this;

	NativeWindow = NULL;
	Resumed = false;
	Ovr = NULL;
	FrameIndex = 1;
	DisplayTime = 0;
	SwapInterval = 1;
	CpuLevel = 2;
	GpuLevel = 2;
	MainThreadTid = 0;
	RenderThreadTid = 0;
	UseMultiview = true;
}

VRAPIRuntime::~VRAPIRuntime(){
}

BBLIB void BBCALL bbUpdateHMD(){
	((VRAPIRuntime*)bbRuntime)->updateTracking();
}

extern "C" int vr_main(int argc,const char *argv[]);

static struct android_app* _app=0;
void android_main( struct android_app* a ){
	_app=a;

	const char *argv[]={ "ovr" };
	vr_main(1, argv);

	ANativeActivity_finish( _app->activity );
	for (;;) {
		int events;
		struct android_poll_source* source;
		if (ALooper_pollAll(-1, NULL, &events, (void**)&source) < 0) {
			break;
		}

		if( source ){
			source->process( _app,source );
		}
	}
}

BBRuntime *bbCreateOpenGLRuntime(){
	java.Vm=_app->activity->vm;
	java.Vm->AttachCurrentThread( &java.Env,NULL );
	java.ActivityObject=_app->activity->clazz;

	// Note that AttachCurrentThread will reset the thread name.
	prctl( PR_SET_NAME,(long)"blitz3d",0,0,0 );

	const ovrInitParms initParms = vrapi_DefaultInitParms(&java);
	if( vrapi_Initialize(&initParms)!=VRAPI_INITIALIZE_SUCCESS ){
		return 0;
	}

	return new VRAPIRuntime( _app );
}

void bbCloseRuntime( BBRuntime *rt ){
	vrapi_Shutdown();
	java.Vm->DetachCurrentThread();

	delete rt;
}

BBMODULE_CREATE( runtime_vrapi ){
	((VRAPIRuntime*)bbRuntime)->init();

	return true;
}

BBMODULE_DESTROY( runtime_vrapi ){
	((VRAPIRuntime*)bbRuntime)->destroy();

	return true;
}
