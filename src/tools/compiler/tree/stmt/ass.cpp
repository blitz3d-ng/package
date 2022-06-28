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

#ifdef USE_LLVM
void AssNode::translate2( Codegen_LLVM *g ){
	var->store2( g,expr->translate2( g ) );
}
#endif

json AssNode::toJSON( Environ *e ){
	json tree;tree["@class"]="AssNode";
	tree["pos"]=pos;
	tree["var"]=var->toJSON( e );
	tree["expr"]=expr->toJSON( e );
	return tree;
}
