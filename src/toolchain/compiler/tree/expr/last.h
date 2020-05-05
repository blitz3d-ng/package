#ifndef LAST_NODE_H
#define LAST_NODE_H

#include "node.h"

struct LastNode : public ExprNode{
	string ident;
	LastNode( const string &i ):ident(i){}
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="LastNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		return tree;
	}
};

#endif
