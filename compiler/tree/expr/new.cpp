#include "new.h"
#include "../../ex.h"

////////////////////
// New expression //
////////////////////
ExprNode *NewNode::semant( Environ *e ){
	sem_type=e->findType( ident );
	if( !sem_type ) ex( "custom type name not found" );
	if( sem_type->structType()==0 ) ex( "type is not a custom type" );
	return this;
}

TNode *NewNode::translate( Codegen *g ){
	return call( "__bbObjNew",global( "_t"+ident ) );
}
