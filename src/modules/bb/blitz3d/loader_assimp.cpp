
#include <bb/blitz/blitz.h>
#include "loader_assimp.h"
#include "meshmodel.h"
#include "meshutil.h"
using namespace std;

static bool conv,flip_tris;
static Transform conv_tform;

MeshModel *Loader_Assimp::parseNode( const struct aiNode* nd ){
	MeshModel *node=d_new MeshModel();
	node->setName( nd->mName.C_Str() );
	// _bbLog( "node: %s\n",nd->mName.C_Str() );

	aiVector3D pos, scl;
	aiQuaternion rot;
	nd->mTransformation.Decompose( scl,rot,pos );

	node->setLocalPosition( Vector( pos[0],pos[1],-pos[2] ) );
	node->setLocalScale( Vector( scl[0],scl[1],scl[2] ) );
	node->setLocalRotation( Quat( rot.w,Vector( rot.x,rot.y,-rot.z ) ) );

	for( int n=0;n<nd->mNumMeshes;++n ){
		const struct aiMesh* mesh=scene->mMeshes[nd->mMeshes[n]];
		const aiMaterial *mtl=scene->mMaterials[mesh->mMaterialIndex];

		Brush b;

		aiColor4D diffuse;
		if( aiGetMaterialColor( mtl,AI_MATKEY_COLOR_DIFFUSE,&diffuse )==AI_SUCCESS ){
			b.setColor( Vector(diffuse.r,diffuse.g,diffuse.b) );
			if( diffuse.a ) b.setAlpha( diffuse.a );
		}

		for( int ti=0;ti<mtl->GetTextureCount( aiTextureType_DIFFUSE );ti++ ){
			aiString path;

			if( mtl->GetTexture( aiTextureType_DIFFUSE,ti,&path )==AI_SUCCESS ){
				Texture tex( path.C_Str(),0 );
				b.setTexture( ti,tex,0 );
				b.setColor( Vector( 1,1,1 ) ); // TODO: this is what the existing loaders do...seems limiting...
			}
		}

		MeshLoader::beginMesh();

		for( int i=0;i<mesh->mNumVertices;++i ){
			Surface::Vertex v;

			v.coords=Vector(&mesh->mVertices[i].x);
			v.coords.z*=-1;
			if( mesh->mColors[0] ) {
				// v.color=Vector(&mesh->mColors[0][i]);
			}
			if( mesh->mNormals ){
				v.normal=Vector(&mesh->mNormals[i].x);
				v.normal.z*=-1;
			}
			if( mesh->HasTextureCoords(0) ){
				v.tex_coords[0][0]=mesh->mTextureCoords[0][i].x;
				v.tex_coords[0][1]=mesh->mTextureCoords[0][i].y;
			}
			if( mesh->HasTextureCoords(1) ){
				v.tex_coords[1][0]=mesh->mTextureCoords[1][i].x;
				v.tex_coords[1][1]=mesh->mTextureCoords[1][i].y;
			}
			MeshLoader::addVertex( v );
		}

		for( int t=0;t<mesh->mNumFaces;++t ) {

			const struct aiFace* face=&mesh->mFaces[t];

			// TODO: fix this
			if( face->mNumIndices!=3 ){
				// cerr<<"[loader.assimp] encountered face with "<<face->mNumIndices<<" indices"<<endl;
				continue;
			}

			int v0=face->mIndices[0],v1=face->mIndices[1],v2=face->mIndices[2];

			if( !flip_tris ){
				MeshLoader::addTriangle( v2,v1,v0,b );
			}else{
				MeshLoader::addTriangle( v0,v1,v2,b );
			}
		}

		MeshModel *m=d_new MeshModel();
		m->setParent( node );
		MeshLoader::endMesh( m );
	}


	for ( int n=0;n<nd->mNumChildren;++n ){
		MeshModel *child=parseNode( nd->mChildren[n] );
		child->setParent( node );
	}

	return node;
}

MeshModel *Loader_Assimp::load( const string &f,const Transform &t,int hint ){
	conv_tform=t;
	conv=flip_tris=false;
	if( conv_tform!=Transform() ){
		conv=true;
		if( conv_tform.m.i.cross(conv_tform.m.j).dot(conv_tform.m.k)<0 ) flip_tris=true;
	}

	Assimp::Importer importer;
	scene=importer.ReadFile( f,aiProcessPreset_TargetRealtime_MaxQuality );
	if( !scene ) return 0;
	return parseNode( scene->mRootNode );
}
