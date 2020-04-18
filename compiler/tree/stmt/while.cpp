#include "while.h"
#include "../expr/const.h"

/////////////////////
// While statement //
/////////////////////
void WhileNode::semant( Environ *e ){
	expr=expr->semant( e );
	expr=expr->castTo( Type::int_type,e );
	string brk=e->setBreak( sem_brk=genLabel() );
	stmts->semant( e );
	e->setBreak( brk );
}

void WhileNode::translate( Codegen *g ){
	string loop=genLabel();
	if( ConstNode *c=expr->constNode() ){
		if( !c->intValue() ) return;
		g->label( loop );
		stmts->translate( g );
		g->code( jump( loop ) );
	}else{
		string cond=genLabel();
		g->code( jump( cond ) );
		g->label( loop );
		stmts->translate( g );
		debug( wendPos,g );
		g->label( cond );
		g->code( jumpt( expr->translate( g ),loop ) );
	}
	g->label( sem_brk );
}
