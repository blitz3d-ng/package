#include "last.h"

///////////////////
// Last of class //
///////////////////
ExprNode *LastNode::semant( Environ *e ){
	sem_type=e->findType( ident );
	if( !sem_type ) ex( "custom type name not found" );
	return this;
}

TNode *LastNode::translate( Codegen *g ){
	return call( "__bbObjLast",global( "_t"+ident ) );
}
