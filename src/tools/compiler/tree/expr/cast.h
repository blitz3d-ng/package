#ifndef CAST_NODE_H
#define CAST_NODE_H

#include "node.h"

struct CastNode : public ExprNode{
	ExprNode *expr;
	Type *type;
	bool implicit;
	CastNode( ExprNode *ex,Type *ty,bool im ):expr( ex ),type( ty ),implicit( im ){}
	~CastNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	llvm::Value *translate2( Codegen_LLVM *g );
#endif
};

#endif
