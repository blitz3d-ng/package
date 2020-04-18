#include "for.h"
#include "../expr/const.h"
#include "../var/node.h"

///////////////////
// For/Next loop //
///////////////////
ForNode::ForNode( VarNode *var,ExprNode *from,ExprNode *to,ExprNode *step,StmtSeqNode *ss,int np )
:var(var),fromExpr(from),toExpr(to),stepExpr(step),stmts(ss),nextPos(np){
}

ForNode::~ForNode(){
	delete stmts;
	delete stepExpr;
	delete toExpr;
	delete fromExpr;
	delete var;
}

void ForNode::semant( Environ *e ){
	var->semant( e );
	Type *ty=var->sem_type;
	if( ty->constType() ) ex( "Index variable can not be constant" );
	if( ty!=Type::int_type && ty!=Type::float_type ){
		ex( "index variable must be integer or real" );
	}
	fromExpr=fromExpr->semant( e );
	fromExpr=fromExpr->castTo( ty,e );
	toExpr=toExpr->semant( e );
	toExpr=toExpr->castTo( ty,e );
	stepExpr=stepExpr->semant( e );
	stepExpr=stepExpr->castTo( ty,e );

	if( !stepExpr->constNode() ) ex( "Step value must be constant" );

	string brk=e->setBreak( sem_brk=genLabel() );
	stmts->semant( e );
	e->setBreak( brk );
}

void ForNode::translate( Codegen *g ){

	TNode *t;Type *ty=var->sem_type;

	//initial assignment
	g->code( var->store( g,fromExpr->translate( g ) ) );

	string cond=genLabel();
	string loop=genLabel();
	g->code( jump( cond ) );
	g->label( loop );
	stmts->translate( g );

	//execute the step part
	debug( nextPos,g );
	int op=ty==Type::int_type ? IR_ADD : IR_FADD;
	t=d_new TNode( op,var->load( g ),stepExpr->translate( g ) );
	g->code( var->store( g,t ) );

	//test for loop cond
	g->label( cond );
	op=stepExpr->constNode()->floatValue()>0 ? '>' : '<';
	t=compare( op,var->load( g ),toExpr->translate( g ),ty );
	g->code( jumpf( t,loop ) );

	g->label( sem_brk );
}


json ForNode::toJSON( Environ *e ){
	json tree;tree["@class"]="ForNode";
	tree["pos"]=pos;
	tree["nextPos"]=nextPos;
	tree["var"]=var->toJSON( e );
	tree["fromExpr"]=fromExpr->toJSON( e );
	tree["toExpr"]=toExpr->toJSON( e );
	tree["stepExpr"]=stepExpr->toJSON( e );
	tree["stmts"]=stmts->toJSON( e );
	tree["sem_brk"]=sem_brk;
	return tree;
}
