#ifndef EXITNODE_H
#define EXITNODE_H

#include "node.h"

struct ExitNode : public StmtNode{
	string sem_brk;
	void semant( Environ *e );
	void translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	llvm::BasicBlock *sem_brk_blk;
	virtual void translate2( Codegen_LLVM *g );
#endif
};

#endif
