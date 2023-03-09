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

#ifdef USE_LLVM
void RepeatNode::translate2( Codegen_LLVM *g ){
	auto current_block = g->builder->GetInsertBlock();
	auto func=g->builder->GetInsertBlock()->getParent();

	auto loop=llvm::BasicBlock::Create( *g->context,"repeat",func );
	auto iter=llvm::BasicBlock::Create( *g->context,"repeat_iter" );
	auto cont=llvm::BasicBlock::Create( *g->context,"repeat_cont" );

	g->builder->CreateBr( loop );
	g->builder->SetInsertPoint( loop );

	auto oldBreakBlock=g->breakBlock;
	g->breakBlock=cont;
	stmts->translate2( g );
	g->builder->CreateBr( iter );
	g->breakBlock=oldBreakBlock;

	func->insert( func->end(),iter );
	g->builder->SetInsertPoint( iter );
	if( expr ) {
		auto cond=expr->translate2( g );
		auto cmp=cond;

		if( !llvm::dyn_cast<llvm::CmpInst>(cmp) ) {
			cmp=compare2( '=',cond,0,expr->sem_type,g );
		}

		g->builder->CreateCondBr( cmp,loop,cont );
	}else{
		g->builder->CreateBr( loop );
	}

	func->insert( func->end(),cont );
	g->builder->SetInsertPoint( cont );
}
#endif

json RepeatNode::toJSON( Environ *e ){
	json tree;tree["@class"]="RepeatNode";
	tree["pos"]=pos;
	tree["untilPos"]=untilPos;
	tree["stmts"]=stmts->toJSON( e );
	if( expr ) tree["expr"]=expr->toJSON( e );
	tree["sem_brk"]=sem_brk;
	return tree;
}
