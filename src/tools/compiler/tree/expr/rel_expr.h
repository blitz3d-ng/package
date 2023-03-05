#ifndef REL_EXPR_NODE_H
#define REL_EXPR_NODE_H

#include "node.h"

//<,=,>,<=,<>,>=
struct RelExprNode : public ExprNode{
	int op;ExprNode *lhs,*rhs;
	Type *opType;
	RelExprNode( int op,ExprNode *lhs,ExprNode *rhs ):op( op ),lhs( lhs ),rhs( rhs ){}
	~RelExprNode(){ delete lhs;delete rhs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	llvm::Value *translate2( Codegen_LLVM *g );
#endif
};

#endif
