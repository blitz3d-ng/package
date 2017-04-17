
#ifndef LOADER_ASSIMP_H
#define LOADER_ASSIMP_H

#include "meshloader.h"

class Loader_Assimp : public MeshLoader{
public:
	MeshModel *load( const string &f,const Transform &conv,int hint );
};

#endif
