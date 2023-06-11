
#ifndef LOADER_X_H
#define LOADER_X_H

#include "meshloader.h"

class Loader_X final : public MeshLoader{
public:
	MeshModel *load( const std::string &f,const Transform &conv,int hint );
};

#endif
