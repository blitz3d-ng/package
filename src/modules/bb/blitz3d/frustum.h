
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "geom.h"

class Frustum{
public:
	enum{
		VERT_TLNEAR=0,VERT_TRNEAR,VERT_BRNEAR,VERT_BLNEAR,
		VERT_TLFAR,VERT_TRFAR,VERT_BRFAR,VERT_BLFAR,VERT_EYE
	};
	enum{
		PLANE_TOP=0,PLANE_LEFT,PLANE_BOTTOM,PLANE_RIGHT,PLANE_NEAR,PLANE_FAR
	};
	Frustum();
	Frustum( float nr,float fr,float w,float h );
	Frustum( float nr,float fr,float l,float r,float u,float d );
	Frustum( const Frustum &f,const Transform &t );

	bool cull( const Box &box )const;
	bool cull( const Vector vecs[],int cnt )const;

	const Plane &getPlane( int n )const{ return planes[n]; }
	const Vector &getVertex( int n )const{ return verts[n]; }

	float getLeft()const;
	float getRight()const;
	float getTop()const;
	float getBottom()const;
	float getNear()const;
	float getFar()const;

private:
	Plane planes[6];
	Vector verts[9];
	void makePlanes();
};

#endif
