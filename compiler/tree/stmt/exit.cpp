#include "exit.h"

///////////
// Break //
///////////
void ExitNode::semant( Environ *e ){
	sem_brk=e->breakLabel;
	if( !sem_brk.size() ) ex( "break must appear inside a loop" );
}

void ExitNode::translate( Codegen *g ){
	g->code( d_new TNode( IR_JUMP,0,0,sem_brk ) );
}
