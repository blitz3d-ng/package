#ifndef BBMESH_H
#define BBMESH_H

class BBMesh{
public:
  virtual bool dirty()const=0; // TODO: figure out if this should be public

  /***** GX INTERFACE *****/
public:
  virtual bool lock( bool all )=0;
  virtual void unlock()=0;

  virtual void setVertex( int n,const void *v )=0;
  virtual void setVertex( int n,const float coords[3],const float normal[3],const float tex_coords[2][2] )=0;
  virtual void setVertex( int n,const float coords[3],const float normal[3],unsigned argb,const float tex_coords[2][2] )=0;
  virtual void setTriangle( int n,int v0,int v1,int v2 )=0;
};

#endif
