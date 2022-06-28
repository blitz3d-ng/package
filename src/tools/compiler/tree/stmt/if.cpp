#include "if.h"
#include "../expr/const.h"

//////////////////
// If statement //
//////////////////
void IfNode::semant( Environ *e ){
	expr=expr->semant( e );
	expr=expr->castTo( Type::int_type,e );
	stmts->semant( e );
	if( elseOpt ) elseOpt->semant( e );
}

void IfNode::translate( Codegen *g ){
	if( ConstNode *c=expr->constNode() ){
		if( c->intValue() ) stmts->translate( g );
		else if( elseOpt ) elseOpt->translate( g );
	}else{
		string _else=genLabel();
		g->code( jumpf( expr->translate( g ),_else ) );
		stmts->translate( g );
		if( elseOpt ){
			string _else2=genLabel();
			g->code( jump( _else2 ) );
			g->label( _else );
			elseOpt->translate( g );
			_else=_else2;
		}
		g->label( _else );
	}
}

#ifdef USE_LLVM
void IfNode::translate2( Codegen_LLVM *g ){
	auto *func = g->builder->GetInsertBlock()->getParent();

	auto then=llvm::BasicBlock::Create( *g->context,"if",func );
	auto cont=llvm::BasicBlock::Create( *g->context,"endif" );

	llvm::BasicBlock *els=0;

	if( elseOpt ){
		els = llvm::BasicBlock::Create( *g->context,"else" );
	}

	auto cond=expr->translate2( g );
	auto cmp=cond;

	if( !llvm::dyn_cast<llvm::CmpInst>(cmp) ) {
		cmp=compare2( NE,cond,0,expr->sem_type,g );
	}

	g->builder->CreateCondBr( cmp,then,els?els:cont );
	g->builder->SetInsertPoint( then );

	stmts->translate2( g );

	auto current_block = g->builder->GetInsertBlock();
	if( !g->builder->GetInsertBlock()->getTerminator() ) {
		g->builder->CreateBr( cont );
	}

	if( els ){
		func->getBasicBlockList().push_back( els );
		g->builder->SetInsertPoint( els );

		elseOpt->translate2( g );

		auto current_block = g->builder->GetInsertBlock();
		if( !llvm::dyn_cast<llvm::BranchInst>(&current_block->back()) ) {
			g->builder->CreateBr( cont );
		}
	}

	func->getBasicBlockList().push_back( cont );
	g->builder->SetInsertPoint( cont );
}
#endif

json IfNode::toJSON( Environ *e ){
	json tree;tree["@class"]="IfNode";
	tree["pos"]=pos;
	tree["expr"]=expr->toJSON( e );
	tree["stmts"]=stmts->toJSON( e );
	if( elseOpt ) tree["elseOpt"]=elseOpt->toJSON( e );
	return tree;
}
