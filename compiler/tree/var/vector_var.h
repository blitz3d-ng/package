#ifndef VECTOR_VAR_NODE_H
#define VECTOR_VAR_NODE_H

#include "node.h"
#include "../expr/node.h"
#include "../expr/expr_seq.h"

struct VectorVarNode : public VarNode{
	ExprNode *expr;
	ExprSeqNode *exprs;
	VectorType *vec_type;
	VectorVarNode( ExprNode *e,ExprSeqNode *es ):expr(e),exprs(es){}
	~VectorVarNode(){ delete expr;delete exprs; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	llvm::Value *load2( Codegen_LLVM *g );
	llvm::Value *translate2( Codegen_LLVM *g );
#endif
};

#endif
