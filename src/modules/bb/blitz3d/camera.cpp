
#include "std.h"
#include "camera.h"
#include "scene.h"

Camera::Camera(){
	setZoom( 1 );
	setRange( 1,1000 );
	setViewport( 0,0,0,0 );
	setClsColor( Vector() );
	setClsMode( true,true );
	setProjMode( PROJ_PERSP );
	setFogRange( 1,1000 );
	setFogColor( Vector() );
	setFogMode( BBScene::FOG_NONE );
}

void Camera::setZoom( float z ){
	zoom=z;
	local_valid=false;
}

void Camera::setRange( float n,float f ){
	frustum_nr=n;frustum_fr=f;
	local_valid=false;
}

void Camera::setViewport( int x,int y,int w,int h ){
	vp_x=x;vp_y=y;vp_w=w;vp_h=h;
	local_valid=false;
}

void Camera::setClsColor( const Vector &v ){
	cls_color=v;
}

void Camera::setClsMode( bool c,bool z ){
	cls_argb=c;cls_z=z;
}

void Camera::setProjMode( int mode ){
	proj_mode=mode;
}

void Camera::setFogColor( const Vector &v ){
	fog_color=v;
}

void Camera::setFogRange( float nr,float fr ){
	fog_nr=nr;fog_fr=fr;
}

void Camera::setFogMode( int mode ){
	fog_mode=mode;
}

void Camera::getViewport( int &x,int &y,int &w,int &h ){
	x=vp_x;y=vp_y;w=vp_w;h=vp_h;
}

const Vector &Camera::getClsColor(){
	return cls_color;
}

void Camera::getClsMode( bool &c,bool &z ){
	c=cls_argb;z=cls_z;
}

int Camera::getProjMode(){
	return proj_mode;
}

const Vector &Camera::getFogColor(){
	return fog_color;
}

float Camera::getFogNear(){
	return fog_nr;
}

float Camera::getFogFar(){
	return fog_fr;
}

int Camera::getFogMode(){
	return fog_mode;
}

const Frustum &Camera::getFrustum()const{
	if( !local_valid ){
		float ar=(float)vp_h/vp_w;
		frustum_w=frustum_nr*2/zoom;
		frustum_h=frustum_nr*2/zoom*ar;
		new( &local_frustum ) Frustum( frustum_nr,frustum_fr,frustum_w,frustum_h );
		local_valid=true;
	}
	return local_frustum;
}

float Camera::getFrustumNear()const{
	return frustum_nr;
}

float Camera::getFrustumFar()const{
	return frustum_fr;
}

float Camera::getFrustumWidth()const{
	getFrustum();return frustum_w;
}

float Camera::getFrustumHeight()const{
	getFrustum();return frustum_h;
}

void Camera::getViewport( int *x,int *y,int *w,int *h )const{
	*x=vp_x;*y=vp_y;*w=vp_w;*h=vp_h;
}
