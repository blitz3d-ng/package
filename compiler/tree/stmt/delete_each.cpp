#include "delete_each.h"

///////////////////////////
// Delete each of a type //
///////////////////////////
void DeleteEachNode::semant( Environ *e ){
	Type *t=e->findType( typeIdent );
	if( !t || t->structType()==0 ) ex( "Specified name is not a NewType name" );
}

void DeleteEachNode::translate( Codegen *g ){
	g->code( call( "__bbObjDeleteEach",global( "_t"+typeIdent ) ) );
}
