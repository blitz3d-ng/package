#include "last.h"

///////////////////
// Last of class //
///////////////////
ExprNode *LastNode::semant( Environ *e ){
	sem_type=e->findType( ident );
	if( !sem_type ) ex( "custom type name not found" );
	return this;
}

TNode *LastNode::translate( Codegen *g ){
	return call( "__bbObjLast",global( "_t"+ident ) );
}

#ifdef USE_LLVM
llvm::Value *LastNode::translate2( Codegen_LLVM *g ){
	llvm::Type *t=sem_type->llvmType( g->context.get() );
	return g->CallIntrinsic( "_bbObjLast",t,1,sem_type->structType()->objty );
}
#endif
