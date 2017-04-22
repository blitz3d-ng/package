
#ifndef LOADER_B3D_H
#define LOADER_B3D_H

#include "meshloader.h"

class Loader_B3D final : public MeshLoader{
public:
	MeshModel *load( const std::string &f,const Transform &conv,int hint );
};

#endif
