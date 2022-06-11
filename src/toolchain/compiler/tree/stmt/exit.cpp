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

#ifdef USE_LLVM
void ExitNode::translate2( Codegen_LLVM *g ){
	auto func=g->builder->GetInsertBlock()->getParent();

	auto exit=llvm::BasicBlock::Create( g->context,"exit",func );
	g->builder->CreateBr( exit );
	g->builder->SetInsertPoint( exit );
	g->builder->CreateBr( g->breakBlock );

	auto cont=llvm::BasicBlock::Create( g->context,"exit",func );
	g->builder->SetInsertPoint( cont );
}
#endif

json ExitNode::toJSON( Environ *e ){
	json tree;tree["@class"]="ExitNode";
	tree["pos"]=pos;
	tree["sem_brk"]=sem_brk;
	return tree;
}
