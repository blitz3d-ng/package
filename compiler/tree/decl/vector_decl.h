#ifndef VECTORDECLNODE_H
#define VECTORDECLNODE_H

#include "node.h"
#include "../expr/expr_seq.h"

struct VectorDeclNode : public DeclNode{
	string ident,tag;
	ExprSeqNode *exprs;
	int kind;
	VectorType *sem_type;
	VectorDeclNode( const string &i,const string &t,ExprSeqNode *e,int k ):ident(i),tag(t),exprs(e),kind(k){}
	~VectorDeclNode(){ delete exprs; }
	void proto( DeclSeq *d,Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e );
};

#endif
