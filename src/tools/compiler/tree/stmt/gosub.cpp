#include "gosub.h"

/////////////////////
// Gosub statement //
/////////////////////
void GosubNode::semant( Environ *e ){
	if( e->level>0 ) ex( "'Gosub' may not be used inside a function" );
	if( !e->findLabel( ident ) ) e->insertLabel( ident,-1,pos,-1 );
	ident=e->funcLabel+ident;
}

void GosubNode::translate( Codegen *g ){
	g->code( jsr( "_l"+ident ) );
}

#ifdef USE_LLVM
void GosubNode::translate2( Codegen_LLVM *g ){
	g->gosubUsed=true;

	auto func=g->builder->GetInsertBlock()->getParent();

	std::string label_cont=ident+"_"+std::string(itoa((bb_int_t)this))+"_cont";
	auto cont=g->getLabel( label_cont );
	func->insert( func->end(),cont );

	g->CallIntrinsic( "_bbPushGosub",g->voidTy,1,llvm::BlockAddress::get( cont ) );
	g->builder->CreateBr( g->getLabel( ident ) );

	g->builder->SetInsertPoint( cont );
}
#endif
