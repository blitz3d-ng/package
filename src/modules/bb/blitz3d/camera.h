
#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "frustum.h"

class Camera : public Object{
public:
	enum{
		PROJ_NONE=0,PROJ_PERSP=1,PROJ_ORTHO=2
	};

	Camera();
	Camera *getCamera(){ return this; }

	//called by user
	void setZoom( float z );
	void setRange( float nr,float fr );
	void setFOV( float left,float right,float up,float down );
	void setViewport( int x,int y,int w,int h );
	void setCanvas( BBCanvas *canvas );
	void setClsColor( const Vector &v );
	void setClsMode( bool cls_argb,bool cls_z );
	void setProjMode( int mode );
	void setFogColor( const Vector &v );
	void setFogRange( float nr,float fr );
	void setFogMode( int mode );

	void  getViewport( int &x,int &y,int &w,int &h );
	BBCanvas *getCanvas();
	const Vector &getClsColor();
	void  getClsMode( bool &c,bool &z );
	int   getProjMode();
	const Vector &getFogColor();
	float getFogNear();
	float getFogFar();
	int   getFogMode();

	//Camera frustum...
	float getFrustumNear()const;
	float getFrustumFar()const;
	float getFrustumWidth()const;
	float getFrustumHeight()const;
	const Frustum &getFrustum()const;
	void  getViewport( int *x,int *y,int *w,int *h )const;
	int   getProjMode()const{ return proj_mode; }

private:
	float zoom;
	int vp_x,vp_y,vp_w,vp_h;
	Vector cls_color;
	bool cls_argb,cls_z;
	int proj_mode;
	Vector fog_color;
	float fog_nr,fog_fr;
	int fog_mode;
	float frustum_nr,frustum_fr;
	float frustum_left,frustum_right,frustum_up,frustum_down;
	BBCanvas *canvas;
	mutable float frustum_w,frustum_h;
	mutable Frustum local_frustum;
	mutable bool local_valid;
};

#endif
