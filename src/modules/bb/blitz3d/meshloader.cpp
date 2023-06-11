
#include "std.h"
#include "meshloader.h"
#include "meshmodel.h"

struct MLTri{
	int verts[3];
};

struct MLSurf{
	std::vector<MLTri> tris;
};

struct MLMesh{
	std::map<Brush,MLSurf*> brush_map;
	std::vector<Surface::Vertex> verts;

	MLMesh(){
	}

	~MLMesh(){
		std::map<Brush,MLSurf*>::const_iterator it;
		for( it=brush_map.begin();it!=brush_map.end();++it ){
			delete it->second;
		}
	}
};

static MLMesh *ml_mesh;
static std::vector<MLMesh*> mesh_stack;

void MeshLoader::beginMesh(){
	mesh_stack.push_back( ml_mesh );
	ml_mesh=d_new MLMesh();
}

int MeshLoader::numVertices(){
	return ml_mesh->verts.size();
}

void MeshLoader::addVertex( const Surface::Vertex &v ){
	ml_mesh->verts.push_back( v );
}

void MeshLoader::addTriangle( const int verts[3],const Brush &b ){
	addTriangle( verts[0],verts[1],verts[2],b );
}

void MeshLoader::addBone( int n,float w,int b ){
	Surface::Vertex &v=ml_mesh->verts[n];
	int i;
	for( i=0;i<MAX_SURFACE_BONES;++i ){
		if( v.bone_bones[i]==255 || w>v.bone_weights[i] ) break;
	}
	if( i==MAX_SURFACE_BONES ) return;
	for( int k=MAX_SURFACE_BONES-1;k>i;--k ){
		v.bone_bones[k]=v.bone_bones[k-1];
		v.bone_weights[k]=v.bone_weights[k-1];
	}
	v.bone_bones[i]=b;
	v.bone_weights[i]=w;
}

Surface::Vertex &MeshLoader::refVertex( int n ){
	return ml_mesh->verts[n];
}

void MeshLoader::addTriangle( int v0,int v1,int v2,const Brush &b ){
	//find surface
	MLSurf *surf;
	std::map<Brush,MLSurf*>::const_iterator it=ml_mesh->brush_map.find( b );
	if( it!=ml_mesh->brush_map.end() ) surf=it->second;
	else{
		surf=d_new MLSurf;
		ml_mesh->brush_map.insert( std::make_pair( b,surf ) );
	}

	MLTri tri;
	tri.verts[0]=v0;tri.verts[1]=v1;tri.verts[2]=v2;
	surf->tris.push_back( tri );
}

void MeshLoader::endMesh( MeshModel *mesh ){
	if( mesh ){
		//fix bone weights
		int k,max_bones=0;
		for( k=0;k<ml_mesh->verts.size();++k ){
			Surface::Vertex &v=ml_mesh->verts[k];
			if( v.bone_bones[0]==255 ) continue;
			int j;
			float t=0;
			for( j=0;j<MAX_SURFACE_BONES;++j ){
				if( v.bone_bones[j]==255 ) break;
				t+=v.bone_weights[j];
			}
			if( j>max_bones ) max_bones=j;
			t=1.0f/t;
			for( j=0;j<MAX_SURFACE_BONES;++j ){
				v.bone_weights[j]*=t;
			}
		}
		std::map<int,int> vert_map;
		std::map<Brush,MLSurf*>::iterator it;
		for( it=ml_mesh->brush_map.begin();it!=ml_mesh->brush_map.end();++it ){
			vert_map.clear();
			Brush b=it->first;
			MLSurf *t=it->second;
			Surface *surf=mesh->findSurface( b );
			if( !surf ) surf=mesh->createSurface( b );
			for( int k=0;k<t->tris.size();++k ){
				Surface::Triangle tri;
				for( int j=0;j<3;++j ){
					int n=t->tris[k].verts[j],id;
					std::map<int,int>::const_iterator it=vert_map.find( n );
					if( it!=vert_map.end() ) id=it->second;
					else{
						id=surf->numVertices();
						surf->addVertex( ml_mesh->verts[n] );
						vert_map.insert( std::make_pair( n,id ) );
					}
					tri.verts[j]=id;
				}
				surf->addTriangle( tri );
			}
		}
	}
	delete ml_mesh;
	ml_mesh=mesh_stack.back();
	mesh_stack.pop_back();
}
