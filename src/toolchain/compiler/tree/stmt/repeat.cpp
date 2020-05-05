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

	auto loop=llvm::BasicBlock::Create( g->context,"repeat",func );
	auto eob=llvm::BasicBlock::Create( g->context,expr?"until":"forever" );

	g->builder->CreateBr( loop );
	g->builder->SetInsertPoint( loop );

	auto oldBreakBlock=g->breakBlock;
	g->breakBlock=eob;
	stmts->translate2( g );
	g->breakBlock=oldBreakBlock;


	if( expr ) {
		auto cond=expr->translate2( g );
		auto cmp=cond;

		if( !dyn_cast<llvm::CmpInst>(cmp) ) {
			cmp=compare2( '=',cond,0,expr->sem_type,g );
		}

		g->builder->CreateCondBr( cmp,loop,eob );
	}else{
		g->builder->CreateBr( loop );
	}

	func->getBasicBlockList().push_back( eob );
	g->builder->SetInsertPoint( eob );
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
