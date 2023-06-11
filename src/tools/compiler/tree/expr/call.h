#ifndef CALL_NODE_H
#define CALL_NODE_H

#include "node.h"
#include "expr_seq.h"

struct CallNode : public ExprNode{
	std::string ident,tag;
	ExprSeqNode *exprs;
	Decl *sem_decl;
	CallNode( const std::string &i,const std::string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~CallNode(){ delete exprs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	llvm::Value *translate2( Codegen_LLVM *g );
#endif
};

#endif
