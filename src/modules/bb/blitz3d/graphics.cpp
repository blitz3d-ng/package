#include "graphics.h"

B3DGraphics::~B3DGraphics(){
	while( scene_set.size() ) freeScene( *scene_set.begin() );
}

BBScene *B3DGraphics::verifyScene( BBScene *s ){
	return scene_set.count( s ) ? s : 0;
}

void B3DGraphics::freeScene( BBScene *scene ){
	if( !scene_set.erase( scene ) ) return;
	delete scene;
}
