#ifndef NEW_NODE_H
#define NEW_NODE_H

#include "node.h"

struct NewNode : public ExprNode{
	string ident;
	NewNode( const string &i ):ident(i){}
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="NewNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		return tree;
	}
};

#endif
