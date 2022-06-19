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
	auto objty=g->builder->CreateBitOrPointerCast( sem_type->structType()->objty,llvm::PointerType::get( g->bbType,0 ) );
	auto t=g->CallIntrinsic( "_bbObjLast",llvm::PointerType::get( g->bbObj,0 ),1,objty );
	return g->builder->CreateBitOrPointerCast( t,llvm::PointerType::get( sem_type->structType()->structtype,0 ) );
}
#endif
