#ifndef NULL_NODE_H
#define NULL_NODE_H

#include "node.h"

struct NullNode : public ExprNode{
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
#ifdef USE_LLVM
	virtual llvm::Value *translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="NullNode";
		return tree;
	}
};

#endif
