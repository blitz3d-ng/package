#include "object_cast.h"

/////////////////
// Object cast //
/////////////////
ExprNode *ObjectCastNode::semant( Environ *e ){
	expr=expr->semant( e );
	expr=expr->castTo( Type::int_type,e );
	sem_type=e->findType( type_ident );
	if( !sem_type ) ex( "custom type name not found" );
	if( !sem_type->structType() ) ex( "type is not a custom type" );
	return this;
}

TNode *ObjectCastNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	t=call( "__bbObjFromHandle",t,global( "_t"+sem_type->structType()->ident ) );
	return t;
}
