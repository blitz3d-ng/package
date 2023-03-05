#include "label.h"

////////////////
// user label //
////////////////
void LabelNode::semant( Environ *e ){
	if( Label *l=e->findLabel( ident ) ){
		if( l->def>=0 ) ex( "duplicate label" );
		l->def=pos;l->data_sz=data_sz;
	}else e->insertLabel( ident,pos,-1,data_sz );
	ident=e->funcLabel+ident;
}

void LabelNode::translate( Codegen *g ){
	g->label( "_l"+ident );
}

#ifdef USE_LLVM
void LabelNode::translate2( Codegen_LLVM *g ){
	auto block=g->getLabel( ident );

	auto func=g->builder->GetInsertBlock()->getParent();
	func->getBasicBlockList().push_back( block );

	g->builder->CreateBr( block );
	g->builder->SetInsertPoint( block );
}
#endif
