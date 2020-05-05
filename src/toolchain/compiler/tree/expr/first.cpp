#include "first.h"

////////////////////
// First of class //
////////////////////
ExprNode *FirstNode::semant( Environ *e ){
	sem_type=e->findType( ident );
	if( !sem_type ) ex( "custom type name name not found" );
	return this;
}

TNode *FirstNode::translate( Codegen *g ){
	return call( "__bbObjFirst",global( "_t"+ident ) );
}
