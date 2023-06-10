
#ifndef MESHCOLLIDER_H
#define MESHCOLLIDER_H

#include "collision.h"

class MeshCollider{
public:
	struct Vertex{
		Vector coords;
	};
	struct Triangle{
		void *surface;
		int verts[3],index;
	};
	MeshCollider( const std::vector<Vertex> &verts,const std::vector<Triangle> &tris );
	~MeshCollider();

	//sphere collision
	bool collide( const Line &line,float radius,Collision *curr_coll,const Transform &tform );

	bool intersects( const MeshCollider &c,const Transform &t )const;

private:
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;

	struct Node{
		Box box;
		Node *left,*right;
		std::vector<int> triangles;
		Node():left(0),right(0){}
		~Node(){ delete left;delete right; }
	};

	Node *tree;
	std::vector<Node*> leaves;

	Box nodeBox( const std::vector<int> &tris );
	Node *createLeaf( const std::vector<int> &tris );
	Node *createNode( const std::vector<int> &tris );
	bool collide( const Box &box,const Line &line,float radius,const Transform &tform,Collision *curr_coll,Node *node );
};

#endif
