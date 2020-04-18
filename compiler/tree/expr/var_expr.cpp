#include "var_expr.h"
#include "const.h"

/////////////////////////
// Variable expression //
/////////////////////////
ExprNode *VarExprNode::semant( Environ *e ){
	var->semant( e );
	sem_type=var->sem_type;
	ConstType *c=sem_type->constType();
	if( !c ) return this;
	ExprNode *expr=constValue( c );
	delete this;return expr;
}

TNode *VarExprNode::translate( Codegen *g ){
	return var->load( g );
}
