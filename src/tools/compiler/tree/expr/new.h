#ifndef NEW_NODE_H
#define NEW_NODE_H

#include "node.h"

struct NewNode : public ExprNode{
	std::string ident;
	NewNode( const std::string &i ):ident(i){}
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
#ifdef USE_LLVM
	virtual llvm::Value *translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="NewNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		return tree;
	}
};

#endif
