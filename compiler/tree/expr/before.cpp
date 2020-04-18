#include "before.h"

////////////////////
// Prev of object //
////////////////////
ExprNode *BeforeNode::semant( Environ *e ){
	expr=expr->semant( e );
	if( expr->sem_type==Type::null_type ) ex( "'Before' cannot be used with 'Null'" );
	if( expr->sem_type->structType()==0 ) ex( "'Before' must be used with a custom type object" );
	sem_type=expr->sem_type;
	return this;
}

TNode *BeforeNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	if( g->debug ) t=jumpf( t,"__bbNullObjEx" );
	return call( "__bbObjPrev",t );
}

json BeforeNode::toJSON( Environ *e ){
	json tree;tree["@class"]="BeforeNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["expr"]=expr->toJSON( e );
	return tree;
}
