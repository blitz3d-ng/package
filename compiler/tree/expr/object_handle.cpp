#include "object_handle.h"

///////////////////
// Object Handle //
///////////////////
ExprNode *ObjectHandleNode::semant( Environ *e ){
	expr=expr->semant( e );
	if( !expr->sem_type->structType() ) ex( "'ObjectHandle' must be used with an object" );
	sem_type=Type::int_type;
	return this;
}

TNode *ObjectHandleNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	return call( "__bbObjToHandle",t );
}
