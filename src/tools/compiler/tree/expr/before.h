#ifndef BEFORE_NODE_H
#define BEFORE_NODE_H

#include "node.h"

struct BeforeNode : public ExprNode{
	ExprNode *expr;
	BeforeNode( ExprNode *e ):expr(e){}
	~BeforeNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
#ifdef USE_LLVM
	virtual llvm::Value *translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e );
};

#endif
