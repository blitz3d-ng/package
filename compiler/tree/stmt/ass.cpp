#include "ass.h"

////////////////
// Assignment //
////////////////
void AssNode::semant( Environ *e ){
	var->semant( e );
	if( var->sem_type->constType() ) ex( "Constants can not be assigned to" );
	if( var->sem_type->vectorType() ) ex( "Blitz arrays can not be assigned to" );
	expr=expr->semant( e );
	expr=expr->castTo( var->sem_type,e );
}

void AssNode::translate( Codegen *g ){
	g->code( var->store( g,expr->translate( g ) ) );
}
