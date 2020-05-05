#include "read.h"

///////////////
// Read data //
///////////////
void ReadNode::semant( Environ *e ){
	var->semant( e );
	if( var->sem_type->constType() ) ex( "Constants can not be modified" );
	if( var->sem_type->structType() ) ex( "Data can not be read into an object" );
}

void ReadNode::translate( Codegen *g ){
	TNode *t;
	if( var->sem_type==Type::int_type ) t=call( "__bbReadInt" );
	else if( var->sem_type==Type::float_type ) t=fcall( "__bbReadFloat" );
	else t=call( "__bbReadStr" );
	g->code( var->store( g,t ) );
}
