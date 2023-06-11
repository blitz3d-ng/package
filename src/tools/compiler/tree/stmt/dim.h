#ifndef DIMNODE_H
#define DIMNODE_H

#include "node.h"
#include "../expr/expr_seq.h"

struct DimNode : public StmtNode{
	std::string ident,tag;
	ExprSeqNode *exprs;
	ArrayType *sem_type;
	Decl *sem_decl;
	DimNode( const std::string &i,const std::string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~DimNode(){ delete exprs; }
	void semant( Environ *e );
	void translate( Codegen *g );
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e );
};

#endif
