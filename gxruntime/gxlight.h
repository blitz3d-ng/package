
#ifndef GXLIGHT_H
#define GXLIGHT_H

#include <bb/blitz3d/light.h>
#include <d3d.h>

class gxLight : public BBLightRep{
public:
	gxLight( int type );
	~gxLight();

	D3DLIGHT7 d3d_light;

	/***** GX INTERFACE *****/
public:
	void update( Light *light );
};

#endif
