
#ifndef LOADER_ASSIMP_H
#define LOADER_ASSIMP_H

#include "meshloader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Loader_Assimp final : public MeshLoader{
protected:
	const aiScene* scene;

	Object *parseNode( const struct aiNode* nd,Object *parent );
public:
	MeshModel *load( const std::string &f,const Transform &conv,int hint );
};

#endif
