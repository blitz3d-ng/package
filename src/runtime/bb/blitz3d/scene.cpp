
#include "scene.h"

BBScene::~BBScene(){
  while( mesh_set.size() ) freeMesh( *mesh_set.begin() );
}

BBMesh *BBScene::verifyMesh( BBMesh *m ){
	return mesh_set.count( m ) ? m : 0;
}

void BBScene::freeMesh( BBMesh *mesh ){
	if( mesh_set.erase( mesh ) ) delete mesh;
}
