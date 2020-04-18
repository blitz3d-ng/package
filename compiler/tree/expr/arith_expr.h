#ifndef ARITH_EXPR_NODE_H
#define ARITH_EXPR_NODE_H

#include "node.h"

// *,/,Mod,+,-
struct ArithExprNode : public ExprNode{
	int op;ExprNode *lhs,*rhs;
	ArithExprNode( int op,ExprNode *lhs,ExprNode *rhs ):op( op ),lhs( lhs ),rhs( rhs ){}
	~ArithExprNode(){ delete lhs;delete rhs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e );
};

#endif
