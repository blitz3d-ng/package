#include "goto.h"

////////////////////
// Goto statement //
////////////////////
void GotoNode::semant( Environ *e ){
	if( !e->findLabel( ident ) ){
		e->insertLabel( ident,-1,pos,-1 );
	}
	ident=e->funcLabel+ident;
}

void GotoNode::translate( Codegen *g ){
	g->code( jump( "_l"+ident ) );
}
