
#include "loader_assimp.h"
#include "meshmodel.h"
#include "meshutil.h"
using namespace std;

static bool conv,flip_tris;
static Transform conv_tform;

MeshModel *Loader_Assimp::parseNode( const struct aiNode* nd ){
	MeshLoader::beginMesh();

	Brush b;

	int tcount=0;

	for ( int n=0;n<nd->mNumMeshes;++n ){
		const struct aiMesh* mesh=scene->mMeshes[nd->mMeshes[n]];
		const aiMaterial *mtl=scene->mMaterials[mesh->mMaterialIndex];

		for( int ti=0;ti<mtl->GetTextureCount( aiTextureType_DIFFUSE );ti++ ){
			aiString path;
			if( mtl->GetTexture( aiTextureType_DIFFUSE,ti,&path )==AI_SUCCESS ){
				Texture tex( path.C_Str(),0 );
				b.setTexture( ti,tex,0 );
			}
		}

		aiColor4D diffuse;
		if( aiGetMaterialColor( mtl,AI_MATKEY_COLOR_DIFFUSE,&diffuse )==AI_SUCCESS ){
			b.setColor( Vector(&diffuse.r) );
			if( diffuse.a ) b.setAlpha( diffuse.a );
		}

		for ( int t=0;t<mesh->mNumFaces;++t ) {

			const struct aiFace* face=&mesh->mFaces[t];

			// TODO: fix this
			if( face->mNumIndices<3 ){
				continue;
			}

			for( int i=0;i<face->mNumIndices;i++ ){
				Surface::Vertex v;

				int vi=face->mIndices[i];

				v.coords=Vector(&mesh->mVertices[vi].x)*Vector(1,1,-1);
				if( mesh->mColors[0] ) {
					// v.color=Vector(&mesh->mColors[0][vi]);
				}
				if( mesh->mNormals ) v.normal=Vector(&mesh->mNormals[vi].x);
				if( mesh->HasTextureCoords(0) ) memcpy( v.tex_coords,&mesh->mTextureCoords[0][vi].x,2*sizeof(float) );
				if( mesh->HasTextureCoords(1) ) memcpy( (&v.tex_coords)+2,&mesh->mTextureCoords[1][vi].x,2*sizeof(float) );

				MeshLoader::addVertex( v );
				tcount++;
			}
			if( !flip_tris ){
				MeshLoader::addTriangle( tcount-1,tcount-2,tcount-3,b );
			}else{
				MeshLoader::addTriangle( tcount-3,tcount-2,tcount-1,b );
			}
		}
	}

	MeshModel *m=d_new MeshModel();

	aiVector3D pos, scl;
	aiQuaternion rot;
	nd->mTransformation.Decompose( scl,rot,pos );

	m->setLocalPosition( Vector( pos[0],pos[1],pos[2] ) );
	m->setLocalScale( Vector( scl[0],scl[1],scl[2] ) );
	m->setLocalRotation( Quat( rot.w,Vector( rot.x,rot.y,rot.z ) ) );

	MeshLoader::endMesh( m );

	for ( int n=0;n<nd->mNumChildren;++n ){
		MeshModel *child=parseNode( nd->mChildren[n] );
		child->setParent( m );
	}

	return m;
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
