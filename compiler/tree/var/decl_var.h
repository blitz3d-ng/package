#ifndef DECL_VAR_NODE_H
#define DECL_VAR_NODE_H

#include "node.h"

struct DeclVarNode : public VarNode{
	Decl *sem_decl;
	DeclVarNode( Decl *d=0 ):sem_decl(d){ if( d ) sem_type=d->type; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );
	virtual TNode *store( Codegen *g,TNode *n );
	bool isObjParam();

	json toJSON( Environ *e );

#ifdef USE_LLVM
	llvm::Value *load2( Codegen_LLVM *g );
	llvm::Value *translate2( Codegen_LLVM *g );
#endif
};

#endif
