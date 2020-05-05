#include "gosub.h"

/////////////////////
// Gosub statement //
/////////////////////
void GosubNode::semant( Environ *e ){
	if( e->level>0 ) ex( "'Gosub' may not be used inside a function" );
	if( !e->findLabel( ident ) ) e->insertLabel( ident,-1,pos,-1 );
	ident=e->funcLabel+ident;
}

void GosubNode::translate( Codegen *g ){
	g->code( jsr( "_l"+ident ) );
}
