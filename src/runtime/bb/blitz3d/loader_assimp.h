
#ifndef LOADER_ASSIMP_H
#define LOADER_ASSIMP_H

#include "meshloader.h"

class Loader_Assimp final : public MeshLoader{
public:
	MeshModel *load( const std::string &f,const Transform &conv,int hint );
};

#endif
