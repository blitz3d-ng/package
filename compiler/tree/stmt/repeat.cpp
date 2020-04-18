#include "repeat.h"
#include "../expr/const.h"

////////////////////////////
// Repeat...Until/Forever //
////////////////////////////
void RepeatNode::semant( Environ *e ){
	sem_brk=genLabel();
	string brk=e->setBreak( sem_brk );
	stmts->semant( e );
	e->setBreak( brk );
	if( expr ){
		expr=expr->semant( e );
		expr=expr->castTo( Type::int_type,e );
	}
}

void RepeatNode::translate( Codegen *g ){

	string loop=genLabel();
	g->label( loop );
	stmts->translate( g );
	debug( untilPos,g );

	if( ConstNode *c=expr ? expr->constNode() : 0 ){
		if( !c->intValue() ) g->code( jump( loop ) );
	}else{
		if( expr ) g->code( jumpf( expr->translate( g ),loop ) );
		else g->code( jump( loop ) );
	}
	g->label( sem_brk );
}
