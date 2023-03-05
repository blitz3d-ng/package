#include "goto.h"

////////////////////
// Goto statement //
////////////////////
void GotoNode::semant( Environ *e ){
	if( !e->findLabel( ident ) ){
		e->insertLabel( ident,-1,pos,-1 );
	}
	ident=e->funcLabel+ident;
}

void GotoNode::translate( Codegen *g ){
	g->code( jump( "_l"+ident ) );
}

#ifdef USE_LLVM
void GotoNode::translate2( Codegen_LLVM *g ){
	g->builder->CreateBr( g->getLabel( ident ) );

	auto func=g->builder->GetInsertBlock()->getParent();
	auto block=llvm::BasicBlock::Create( *g->context,"",func );
	g->builder->SetInsertPoint( block );
}
#endif
