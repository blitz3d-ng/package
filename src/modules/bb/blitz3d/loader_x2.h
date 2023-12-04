
#ifndef LOADER_X2_H
#define LOADER_X2_H

#include "meshloader.h"

class Loader_X2 final : public MeshLoader{
public:
	MeshModel *load( const std::string &f,const Transform &conv,int hint );
};

#endif
