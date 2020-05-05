#include "for_each.h"

///////////////////////////////
// For each object of a type //
///////////////////////////////
void ForEachNode::semant( Environ *e ){
	var->semant( e );
	Type *ty=var->sem_type;

	if( ty->structType()==0 ) ex( "Index variable is not a NewType" );
	Type *t=e->findType( typeIdent );
	if( !t ) ex( "Type name not found" );
	if( t!=ty ) ex( "Type mismatch" );

	string brk=e->setBreak( sem_brk=genLabel() );
	stmts->semant( e );
	e->setBreak( brk );
}

void ForEachNode::translate( Codegen *g ){
	TNode *t,*l,*r;
	string _loop=genLabel();

	string objFirst,objNext;

	if( var->isObjParam() ){
		objFirst="__bbObjEachFirst2";
		objNext="__bbObjEachNext2";
	}else{
		objFirst="__bbObjEachFirst";
		objNext="__bbObjEachNext";
	}

	l=var->translate( g );
	r=global( "_t"+typeIdent );
	t=jumpf( call( objFirst,l,r ),sem_brk );
	g->code( t );

	g->label( _loop );
	stmts->translate( g );

	debug( nextPos,g );
	t=jumpt( call( objNext,var->translate( g ) ),_loop );
	g->code( t );

	g->label( sem_brk );
}
