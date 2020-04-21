#ifndef VAR_NODE_H
#define VAR_NODE_H

#include "../node.h"

struct VarNode : public Node{
	Type *sem_type;

	//get set var
	TNode *load( Codegen *g );
	virtual TNode *store( Codegen *g,TNode *n );
	virtual bool isObjParam();

	//addr of var
	virtual void semant( Environ *e )=0;
	virtual TNode *translate( Codegen *g )=0;

#ifdef USE_LLVM
	virtual llvm::Value *load2( Codegen_LLVM *g );
	virtual void store2( Codegen_LLVM *g,llvm::Value *v );
	virtual llvm::Value *translate2( Codegen_LLVM *g ); // TODO: make abstract once everything is implemented
#endif
};

#endif
