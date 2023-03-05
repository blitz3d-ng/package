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
	auto objty=g->builder->CreateBitOrPointerCast( sem_type->structType()->objty,llvm::PointerType::get( g->bbType,0 ) );
	auto t=g->CallIntrinsic( "_bbObjFirst",llvm::PointerType::get( g->bbObj,0 ),1,objty );
	return g->builder->CreateBitOrPointerCast( t,llvm::PointerType::get( sem_type->structType()->structtype,0 ) );
}
#endif
