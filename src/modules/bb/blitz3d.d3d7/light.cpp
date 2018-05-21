
#include "light.h"

gxLight::gxLight( int type ):BBLightRep(){

	memset(&d3d_light,0,sizeof(d3d_light));

	switch( type ){
	case Light::LIGHT_POINT:
		d3d_light.dltType=D3DLIGHT_POINT;
		break;
	case Light::LIGHT_SPOT:
		d3d_light.dltType=D3DLIGHT_SPOT;
		break;
	default:
		d3d_light.dltType=D3DLIGHT_DIRECTIONAL;
	}

	d3d_light.dcvDiffuse.a=1;
	d3d_light.dcvSpecular.r=d3d_light.dcvSpecular.g=d3d_light.dcvSpecular.b=1;
	d3d_light.dvRange=D3DLIGHT_RANGE_MAX;
	d3d_light.dvFalloff=1;
}

gxLight::~gxLight(){
}

void gxLight::update( Light *l ){
	Vector pos( l->getRenderTform().v ),dir( l->getRenderTform().m.k );

	memcpy( &d3d_light.dvPosition,&pos.x,12 );
	memcpy( &d3d_light.dvDirection,&dir.x,12 );

	if( dirty ){
		d3d_light.dvAttenuation1=1.0f/l->getRange();
		memcpy( &d3d_light.dcvDiffuse,&l->getColor().x,12 );
		l->getConeAngles( d3d_light.dvTheta,d3d_light.dvPhi );
		dirty=false;
	}
}
