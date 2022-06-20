#include "restore.h"

//////////////////
// Restore data //
//////////////////
void RestoreNode::semant( Environ *e ){
	if( e->level>0 ) e=e->globals;

	if( ident.size()==0 ) sem_label=0;
	else{
		sem_label=e->findLabel( ident );
		if( !sem_label ) sem_label=e->insertLabel( ident,-1,pos,-1 );
	}
}

void RestoreNode::translate( Codegen *g ){
	TNode *t=global( "__DATA" );
	if( sem_label ) t=add( t,iconst( sem_label->data_sz*8 ) );
	g->code( call( "__bbRestore",t ) );
}

#ifdef USE_LLVM
void RestoreNode::translate2( Codegen_LLVM *g ){
	llvm::Type *void_ty=llvm::Type::getVoidTy( *g->context );
	llvm::Type *int_ty=Type::int_type->llvmType( g->context.get() );

	llvm::Value *t=g->bbData;
	if( sem_label ){
		cout<<sem_label->data_sz<<endl;
		vector<llvm::Value*> indices;
		indices.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 32,sem_label->data_sz*2 ) ) );
		t=g->builder->CreateGEP( int_ty,t,indices );
	}

	g->CallIntrinsic( "_bbRestore",void_ty,1,g->builder->CreateBitOrPointerCast( t,llvm::PointerType::get( int_ty,0 ) ) );
}
#endif
