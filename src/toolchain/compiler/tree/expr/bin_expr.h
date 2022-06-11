#ifndef BIN_EXPR_NODE_H
#define BIN_EXPR_NODE_H

#include "node.h"

// and, or, eor, lsl, lsr, asr
struct BinExprNode : public ExprNode{
	int op;ExprNode *lhs,*rhs;
	BinExprNode( int op,ExprNode *lhs,ExprNode *rhs ):op( op ),lhs( lhs ),rhs( rhs ){}
	~BinExprNode(){ delete lhs;delete rhs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
#ifdef USE_LLVM
	llvm::Value *translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e );
};

#endif
