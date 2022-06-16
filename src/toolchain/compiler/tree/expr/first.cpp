#include "first.h"

////////////////////
// First of class //
////////////////////
ExprNode *FirstNode::semant( Environ *e ){
	sem_type=e->findType( ident );
	if( !sem_type ) ex( "custom type name name not found" );
	return this;
}

TNode *FirstNode::translate( Codegen *g ){
	return call( "__bbObjFirst",global( "_t"+ident ) );
}

#ifdef USE_LLVM
llvm::Value *FirstNode::translate2( Codegen_LLVM *g ){
	llvm::Type *t=sem_type->llvmType( g->context.get() );
	return g->CallIntrinsic( "_bbObjFirst",t,1,sem_type->structType()->objty );
}
#endif
