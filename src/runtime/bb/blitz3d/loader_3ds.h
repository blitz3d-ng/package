
#ifndef LOADER_3DS_H
#define LOADER_3DS_H

#include "meshloader.h"

class Loader_3DS final : public MeshLoader{
public:
	MeshModel *load( const std::string &f,const Transform &conv,int hint );
};

#endif
