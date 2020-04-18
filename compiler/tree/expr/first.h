#ifndef FIRST_NODE_H
#define FIRST_NODE_H

#include "node.h"

struct FirstNode : public ExprNode{
	string ident;
	FirstNode( const string &i ):ident(i){}
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="FirstNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		return tree;
	}
};

#endif
