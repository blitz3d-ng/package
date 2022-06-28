#ifndef UNI_EXPR_NODE_H
#define UNI_EXPR_NODE_H

#include "node.h"

struct UniExprNode : public ExprNode{
	int op;ExprNode *expr;
	UniExprNode( int op,ExprNode *expr ):op( op ),expr( expr ){}
	~UniExprNode(){ delete expr; }
	ExprNode *constize();
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );
#ifdef USE_LLVM
	llvm::Value *translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="UniExprNode";
		tree["sem_type"]=sem_type->toJSON();
		switch( op ){
		case '-':tree["op"]="SUB";break;
		case '+':tree["op"]="ADD";break;
		case ABS:tree["op"]="ABS";break;
		case SGN:tree["op"]="SGN";break;
		}
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

#endif
