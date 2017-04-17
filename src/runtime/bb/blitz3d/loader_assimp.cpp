
#include "loader_assimp.h"
#include "meshmodel.h"
#include "meshutil.h"

MeshModel *Loader_Assimp::load( const string &f,const Transform &conv,int hint ){
  Brush b;
  b.setColor( Vector( 1,0,0 ) );
  MeshModel *root=MeshUtil::createSphere( b,16 );
  return root;
}
