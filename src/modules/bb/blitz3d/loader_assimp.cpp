
#include <bb/blitz/blitz.h>
#include "loader_assimp.h"
#include "meshmodel.h"
#include "meshutil.h"
#include "pivot.h"

#include <map>

// #define SHOW_BONES

static bool conv,flip_tris;
static Transform conv_tform;
static bool collapse,animonly;

static std::map<std::string,Object*> obj_map;
static std::vector<Object*> bones;

inline Vector convertPos( aiVector3D &pos ){
	return Vector( pos.x,pos.y,-pos.z );
}

inline Vector convertVec( aiVector3D &scl ){
	return Vector( scl.x,scl.y,scl.z );
}

inline Quat convertQuat( aiQuaternion &rot ){
	return Quat( rot.w,Vector( rot.x,rot.y,-rot.z ) );
}

inline void reset(){
	obj_map.clear();
	bones.clear();
}

static Object *allocBone(){
#ifdef SHOW_BONES
	Brush b;
	b.setColor( Vector( 1,0,0 ) );
	b.setAlpha( .75f );
	MeshModel *bone=MeshUtil::createSphere( b,16 );
	Transform t;
	t.m.i.x=.4f;
	t.m.j.y=.4f;
	t.m.k.z=.4f;
	bone->transform( t );
#else
	Pivot *bone=d_new Pivot();
#endif

	return bone;
}

Object *Loader_Assimp::parseNode( const struct aiNode* nd,Object *parent ){
	MeshModel *mesh=0;
	Object *obj=0;
	std::string name=nd->mName.C_Str();

	if( nd->mNumMeshes>0 ){
		obj=mesh=d_new MeshModel();

		if( !animonly ){
			bool normals=true;
			MeshLoader::beginMesh();

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

				int vertex_offset=MeshLoader::numVertices();

				for( int i=0;i<mesh->mNumVertices;++i ){
					Surface::Vertex v;

					v.coords=convertPos( mesh->mVertices[i] );
					if( mesh->mColors[0] ) {
						// v.color=Vector(&mesh->mColors[0][i]);
					}
					if( mesh->mNormals ){
						v.normal=convertPos( mesh->mNormals[i] );
					}else{
						normals=false;
					}
					if( mesh->HasTextureCoords(0) ){
						v.tex_coords[0][0]=mesh->mTextureCoords[0][i].x;
						v.tex_coords[0][1]=-mesh->mTextureCoords[0][i].y;
					}
					if( mesh->HasTextureCoords(1) ){
						v.tex_coords[1][0]=mesh->mTextureCoords[1][i].x;
						v.tex_coords[1][1]=-mesh->mTextureCoords[1][i].y;
					}
					MeshLoader::addVertex( v );
				}

				for( int t=0;t<mesh->mNumFaces;++t ) {
					const struct aiFace* face=&mesh->mFaces[t];

					// TODO: what to do here?
					if( face->mNumIndices!=3 ){
						// _bbLog( "[loader.assimp] encountered face with %i indices\n",face->mNumIndices );
						continue;
					}

					int v0=face->mIndices[0],v1=face->mIndices[1],v2=face->mIndices[2];

					if( flip_tris ){
						MeshLoader::addTriangle( vertex_offset+v0,vertex_offset+v1,vertex_offset+v2,b );
					}else{
						MeshLoader::addTriangle( vertex_offset+v2,vertex_offset+v1,vertex_offset+v0,b );
					}
				}

				for( int bi=0;bi<mesh->mNumBones;bi++ ){
					const aiBone *b=mesh->mBones[bi];

					Object *bone=obj_map[b->mName.C_Str()];
					if( bone==0 ){
						bone=allocBone();
						bones.push_back( bone );

						obj_map[b->mName.C_Str()]=bone;
					}

					for( int wi=0;wi<b->mNumWeights;wi++ ){
						int vert=b->mWeights[wi].mVertexId;
						float weight=b->mWeights[wi].mWeight;
						MeshLoader::addBone( vertex_offset+vert,weight,bones.size() );
					}
				}
			}

			MeshLoader::endMesh( mesh );
			if( !normals ) mesh->updateNormals();
		}
	}else{
		obj=obj_map[name];
		if( !obj ) obj=allocBone();
	}

	obj_map[name]=obj;

	aiVector3D pos, scl;
	aiQuaternion rot;
	nd->mTransformation.Decompose( scl,rot,pos );

	obj->setName( name );
	obj->setLocalPosition( convertPos( pos ) );
	obj->setLocalScale( convertVec( scl ) );
	obj->setLocalRotation( convertQuat( rot ) );

	for ( int n=0;n<nd->mNumChildren;++n ){
		parseNode( nd->mChildren[n],obj );
	}

	if( parent ){
		obj->setParent( parent );
	}else if( !mesh ) {
		mesh=d_new MeshModel();
		obj->setParent( mesh );
		obj=mesh;
	}

	return obj;
}

MeshModel *Loader_Assimp::load( const std::string &f,const Transform &t,int hint ){
	conv_tform=t;
	conv=flip_tris=false;
	if( conv_tform!=Transform() ){
		conv=true;
		if( conv_tform.m.i.cross(conv_tform.m.j).dot(conv_tform.m.k)<0 ) flip_tris=true;
	}

	// TODO: apply tforms...
	flip_tris=false;

	collapse=!!(hint&MeshLoader::HINT_COLLAPSE);
	animonly=!!(hint&MeshLoader::HINT_ANIMONLY);

	Assimp::Importer importer;
	scene=importer.ReadFile( f,aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_Triangulate|aiProcess_JoinIdenticalVertices );
	if( !scene ) return 0;

	reset();

	Object *obj=parseNode( scene->mRootNode,0 );

	MeshModel *mesh=obj->getModel()->getMeshModel();

	if( !collapse ){
		float anim_len=0.0;
		for( int i=0;i<scene->mNumAnimations;i++ ){
			aiAnimation *a=scene->mAnimations[i];

			if( a->mNumMeshChannels ) _bbLog( "vertex-based animations are not support in loader\n" );

			std::map<Object*,Animation> anim;
			for( int j=0;j<a->mNumChannels;j++ ){
				aiNodeAnim *c=a->mChannels[j];

				Object *obj=obj_map[c->mNodeName.C_Str()];
				if( bones.size() && std::count( bones.begin(),bones.end(),obj )==0 ) {
					bones.push_back( obj );
				}

				Animation anim=obj->getAnimation();

				for( int k=0;k<c->mNumPositionKeys;k++ ){
					aiVectorKey key=c->mPositionKeys[k];
					anim.setPositionKey( key.mTime,convertPos( key.mValue ) );
				}

				for( int k=0;k<c->mNumRotationKeys;k++ ){
					aiQuatKey key=c->mRotationKeys[k];
					anim.setRotationKey( key.mTime,convertQuat( key.mValue ) );
				}

				for( int k=0;k<c->mNumScalingKeys;k++ ){
					aiVectorKey key=c->mScalingKeys[k];
					anim.setScaleKey( key.mTime,convertVec( key.mValue ) );
				}

				obj->setAnimation( anim );
			}

			anim_len=a->mDuration;
		}

		if( mesh && bones.size() ){
			bones.insert( bones.begin(),mesh );
			mesh->setAnimator( d_new Animator( bones,anim_len ) );
			mesh->createBones();
			bones.clear();
		}else if( anim_len ){
			obj->setAnimator( d_new Animator( obj,anim_len ) );
		}
	}

	reset();

	return mesh;
}
