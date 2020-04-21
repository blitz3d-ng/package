#ifndef DECLSTMTNODE_H
#define DECLSTMTNODE_H

#include "node.h"
#include "../decl/node.h"

struct DeclStmtNode : public StmtNode{
	DeclNode *decl;
	DeclStmtNode( DeclNode *d ):decl(d){ pos=d->pos; }
	~DeclStmtNode(){ delete decl; }
	void semant( Environ *e );
	void translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif
};

#endif
