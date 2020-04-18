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

	json toJSON( Environ *e ){
		json tree;tree["@class"]="RelExprNode";
		tree["sem_type"]=sem_type->toJSON();
		switch( op ){
		case '<': tree["op"]="LT";break;
		case '=': tree["op"]="EQ";break;
		case '>': tree["op"]="GT";break;
		case LE: tree["op"]="LE";break;
		case NE: tree["op"]="NE";break;
		case GE: tree["op"]="GE";break;
		}
		tree["lhs"]=lhs->toJSON( e );
		tree["rhs"]=rhs->toJSON( e );
		tree["opType"]=opType->toJSON();
		return tree;
	}
};

#endif
