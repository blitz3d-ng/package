
#include "scene.h"
#include "camera.h"

BBScene::~BBScene(){
  while( mesh_set.size() ) freeMesh( *mesh_set.begin() );
}

BBMesh *BBScene::verifyMesh( BBMesh *m ){
	return mesh_set.count( m ) ? m : 0;
}

void BBScene::freeMesh( BBMesh *mesh ){
	if( mesh_set.erase( mesh ) ) delete mesh;
}

bool BBScene::setCamera( Camera *cam,int cw,int ch ){
	if( !cam->getProjMode() ) return false;

	setCanvas( cw,ch );

	int x,y,w,h;
	cam->getViewport( x,y,w,h );
	setViewport( x,y,w,h );

	bool cls_argb,cls_z;
	cam->getClsMode( cls_argb,cls_z );
	clear( &(cam->getClsColor().x),1,1,cls_argb,cls_z );

	const Frustum frustum=cam->getFrustum();
	float frustum_nr=cam->getFrustumNear(),frustum_fr=cam->getFrustumFar(),
	      frustum_l=frustum.getLeft(),frustum_r=frustum.getRight(),
	      frustum_t=frustum.getTop(),frustum_b=frustum.getBottom();

	if( cam->getProjMode()==Camera::PROJ_ORTHO ){
		setOrthoProj( frustum_nr,frustum_fr,frustum_l,frustum_r,frustum_t,frustum_b );
	}else{
		setPerspProj( frustum_nr,frustum_fr,frustum_l,frustum_r,frustum_t,frustum_b );
	}

	setFogRange( cam->getFogNear(),cam->getFogFar() );
	setFogColor( (float*)&cam->getFogColor().x );
	setFogMode( cam->getFogMode() );

	return true;
}
