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

#ifdef USE_LLVM
llvm::Value *VarExprNode::translate2( Codegen_LLVM *g ){
	return var->load2( g );
}
#endif

json VarExprNode::toJSON( Environ *e ){
	json tree;tree["@class"]="VarExprNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["var"]=var->toJSON( e );
	return tree;
}
