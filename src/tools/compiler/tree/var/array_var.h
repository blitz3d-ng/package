#ifndef ARRAY_VAR_NODE_H
#define ARRAY_VAR_NODE_H

#include "node.h"
#include "../expr/expr_seq.h"

struct ArrayVarNode : public VarNode{
	string ident,tag;
	ExprSeqNode *exprs;
	Decl *sem_decl;
	ArrayVarNode( const string &i,const string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~ArrayVarNode(){ delete exprs; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );
#ifdef USE_LLVM
	virtual llvm::Value *translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e );
};

#endif
