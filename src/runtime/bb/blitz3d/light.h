
#ifndef LIGHT_H
#define LIGHT_H

#include "geom.h"
#include "object.h"

class BBLightRep{
protected:
	bool dirty;

	BBLightRep();
	virtual ~BBLightRep();
public:
	void markDirty();
	virtual void update( Light *light )=0;
};

class World;

class Light : public Object{
public:
	enum{
		LIGHT_DISTANT=1,LIGHT_POINT=2,LIGHT_SPOT=3
	};

	Light( int type );
	~Light();

	Light *getLight(){ return this; }

	float getRange();
	void setRange( float r );
	const Vector &getColor();
	void setColor( const Vector &v );
	void getConeAngles( float *inner,float *outer );
	void setConeAngles( float inner,float outer );

	bool beginRender( float tween );

	BBLightRep *getRep()const{ return rep; }

private:
	friend class World;
	BBLightRep *rep;

	int type;
	float range;
	Vector color;
	float innerAngle,outerAngle;
};

#endif
