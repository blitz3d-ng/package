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

#ifdef USE_LLVM
void WhileNode::translate2( Codegen_LLVM *g ){
	ConstNode *c=expr->constNode();
	if( c && !c->intValue() ) return;

	auto current_block = g->builder->GetInsertBlock();
	auto func=g->builder->GetInsertBlock()->getParent();

	auto loop=llvm::BasicBlock::Create( *g->context,"while",func );
	auto next=llvm::BasicBlock::Create( *g->context,"wend" );

	g->builder->CreateBr( loop );
	g->builder->SetInsertPoint( loop );

	if( !c ) {
		auto body=llvm::BasicBlock::Create( *g->context,"body" );

		auto v=expr->translate2( g );
		v=g->builder->CreateIntCast( v,llvm::Type::getInt1Ty( *g->context ),true ); // TODO: this is a hack and hopefully optimized out...

		g->builder->CreateCondBr( v,body,next );

		func->getBasicBlockList().push_back( body );
		g->builder->SetInsertPoint( body );
	}

	auto oldBreakBlock=g->breakBlock;
	g->breakBlock=next;
	stmts->translate2( g );
	g->breakBlock=oldBreakBlock;

	g->builder->CreateBr( loop );

	func->getBasicBlockList().push_back( next );
	g->builder->SetInsertPoint( next );
}
#endif

json WhileNode::toJSON( Environ *e ){
	json tree;tree["@class"]="WhileNode";
	tree["pos"]=pos;
	tree["wendPos"]=wendPos;
	tree["expr"]=expr->toJSON( e );
	tree["stmts"]=stmts->toJSON( e );
	tree["sem_brk"]=sem_brk;
	return tree;
}
