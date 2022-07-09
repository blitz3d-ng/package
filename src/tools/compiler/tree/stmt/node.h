
#ifndef STMTNODE_H
#define STMTNODE_H

#include "../node.h"

struct StmtNode : public Node{
	int pos;	//offset in source stream
	StmtNode():pos(-1){}
	void debug( int pos,Codegen *g );
#ifdef USE_LLVM
	void debug2( int pos,Codegen_LLVM *g );
#endif

	virtual void semant( Environ *e ){}
	virtual void translate( Codegen *g ){}
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif
};

#endif
