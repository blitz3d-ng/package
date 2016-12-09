
#ifndef GXLIGHT_H
#define GXLIGHT_H

#include "../blitz3d/light.h"

class gxScene;

class gxLight : public BBLightRep{
public:
	gxLight( gxScene *scene,int type );
	~gxLight();

	D3DLIGHT7 d3d_light;

private:
	gxScene *scene;

	/***** GX INTERFACE *****/
public:
	void update( Light *light );
};

#endif
