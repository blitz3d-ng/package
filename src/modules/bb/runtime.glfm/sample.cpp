#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glfm.h>
#define FILE_COMPAT_ANDROID_ACTIVITY glfmAndroidGetActivity()
#include "file_compat.h"

#include <bb/blitz/commands.h>
#include <bb/math/commands.h>
#include <bb/graphics/commands.h>
#include <bb/blitz2d/commands.h>
#include <bb/blitz3d/commands.h>

typedef struct {
    double lastTouchX;
    double lastTouchY;

    double offsetX;
    double offsetY;
} ExampleApp;

static void onFrame(GLFMDisplay *display, double frameTime);
static void onSurfaceCreated(GLFMDisplay *display, int width, int height);
static void onSurfaceDestroyed(GLFMDisplay *display);
static bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y);
static bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers);

int StartupIOS( GLFMDisplay *display );

void glfmMain( GLFMDisplay *display ) {
	printf("Booting...\n");
	StartupIOS( display );

	printf("Hello from iOS! RndSeed = %i\n", bbRndSeed());

	ExampleApp *app = (ExampleApp*)calloc(1, sizeof(ExampleApp));

	glfmSetDisplayConfig(display,
	                     GLFMRenderingAPIOpenGLES2,
	                     GLFMColorFormatRGBA8888,
	                     GLFMDepthFormatNone,
	                     GLFMStencilFormatNone,
	                     GLFMMultisampleNone);
	glfmSetUserData(display, app);
	glfmSetSurfaceCreatedFunc(display, onSurfaceCreated);
	glfmSetSurfaceResizedFunc(display, onSurfaceCreated);
	glfmSetSurfaceDestroyedFunc(display, onSurfaceDestroyed);
	glfmSetMainLoopFunc(display, onFrame);
	glfmSetTouchFunc(display, onTouch);
	glfmSetKeyFunc(display, onKey);
}

static bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y) {
    if (phase == GLFMTouchPhaseHover) {
        return false;
    }
    ExampleApp *app = (ExampleApp*)glfmGetUserData(display);
    if (phase != GLFMTouchPhaseBegan) {
        int width, height;
        glfmGetDisplaySize(display, &width, &height);
        app->offsetX += 2 * (x - app->lastTouchX) / width;
        app->offsetY -= 2 * (y - app->lastTouchY) / height;
    }
    app->lastTouchX = x;
    app->lastTouchY = y;
    return true;
}

static bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers) {
    bool handled = false;
    if (action == GLFMKeyActionPressed) {
        ExampleApp *app = (ExampleApp*)glfmGetUserData(display);
        switch (keyCode) {
            case GLFMKeyLeft:
                app->offsetX -= 0.1f;
                handled = true;
                break;
            case GLFMKeyRight:
                app->offsetX += 0.1f;
                handled = true;
                break;
            case GLFMKeyUp:
                app->offsetY += 0.1f;
                handled = true;
                break;
            case GLFMKeyDown:
                app->offsetY -= 0.1f;
                handled = true;
                break;
            default:
                break;
        }
    }
    return handled;
}

Entity *cube=0;

static void onSurfaceCreated(GLFMDisplay *display, int width, int height) {
	bbGraphics3D( width,height,0,0 );
	Camera *cam=(Camera*)bbCreateCamera( 0 );
	bbCameraClsColor( cam,0,100,100 );

	cube=bbCreateCube( 0 );
	bbPositionEntity( cube,0.0f,0.0f,5.0f,1 );

	// bb_int_t tex=bbLoadTexture( "crate.jpg",0 );
	// if (tex) {
	// 	bbEntityTexture( cube,tex,0,0 );
	// } else {
	// 	printf("Failed to load teture.\n");
	// }

	printf("Hello from GLFM! Screen size %ix%i\n", width, height);

	glViewport(0, 0, width, height);

	GLFMRenderingAPI api = glfmGetRenderingAPI(display);
	printf("Hello from GLFM! Using OpenGL %s\n",
		api == GLFMRenderingAPIOpenGLES32 ? "ES 3.2" :
		api == GLFMRenderingAPIOpenGLES31 ? "ES 3.1" :
		api == GLFMRenderingAPIOpenGLES3 ? "ES 3.0" : "ES 2.0");
}

static void onSurfaceDestroyed(GLFMDisplay *display) {
	// TODO: wipe gl stuff...
}

static void onFrame(GLFMDisplay *display, double frameTime) {
	ExampleApp *app = (ExampleApp*)glfmGetUserData(display);

	bbTurnEntity( cube,0.0f,0.1f,0.0f,1 );
	bbRenderWorld( 0.0f );
	bbRect( 100,100,100,100,1 );
}
