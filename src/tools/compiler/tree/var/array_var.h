#ifndef ARRAY_VAR_NODE_H
#define ARRAY_VAR_NODE_H

#include "node.h"
#include "../expr/expr_seq.h"

struct ArrayVarNode : public VarNode{
	std::string ident,tag;
	ExprSeqNode *exprs;
	Decl *sem_decl;
	ArrayVarNode( const std::string &i,const std::string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~ArrayVarNode(){ delete exprs; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );
#ifdef USE_LLVM
	virtual llvm::Value *translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e );
};

#endif
