#ifndef VECTORDECLNODE_H
#define VECTORDECLNODE_H

#include "node.h"
#include "../expr/expr_seq.h"

struct VectorDeclNode : public DeclNode{
	std::string ident,tag;
	ExprSeqNode *exprs;
	int kind;
	VectorType *sem_type;
	VectorDeclNode( const std::string &i,const std::string &t,ExprSeqNode *e,int k ):ident(i),tag(t),exprs(e),kind(k){}
	~VectorDeclNode(){ delete exprs; }
	void proto( DeclSeq *d,Environ *e );
	void translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
