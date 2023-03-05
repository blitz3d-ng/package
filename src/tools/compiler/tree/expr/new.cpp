#include "new.h"
#include "../../ex.h"

////////////////////
// New expression //
////////////////////
ExprNode *NewNode::semant( Environ *e ){
	sem_type=e->findType( ident );
	if( !sem_type ) ex( "custom type name not found" );
	if( sem_type->structType()==0 ) ex( "type is not a custom type" );
	return this;
}

TNode *NewNode::translate( Codegen *g ){
	return call( "__bbObjNew",global( "_t"+ident ) );
}

#ifdef USE_LLVM
llvm::Value *NewNode::translate2( Codegen_LLVM *g ){
	auto objty=g->builder->CreateBitOrPointerCast( sem_type->structType()->objty,llvm::PointerType::get( g->bbObjType,0 ) );

	auto ty=sem_type->llvmType( g->context.get() );
	auto t=g->CallIntrinsic( "_bbObjNew",llvm::PointerType::get( g->bbObj,0 ),1,objty );
	return g->builder->CreateBitOrPointerCast( t,ty );
}
#endif
