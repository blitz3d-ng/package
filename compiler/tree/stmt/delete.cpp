#include "delete.h"

//////////////////////
// Delete statement //
//////////////////////
void DeleteNode::semant( Environ *e ){
	expr=expr->semant( e );
	if( expr->sem_type->structType()==0 ) ex( "Can't delete non-Newtype" );
}

void DeleteNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	g->code( call( "__bbObjDelete",t ) );
}
