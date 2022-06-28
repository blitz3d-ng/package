#include "delete_each.h"

///////////////////////////
// Delete each of a type //
///////////////////////////
void DeleteEachNode::semant( Environ *e ){
	Type *t=e->findType( typeIdent );
	if( !t || t->structType()==0 ) ex( "Specified name is not a NewType name" );
	type=t;
}

void DeleteEachNode::translate( Codegen *g ){
	g->code( call( "__bbObjDeleteEach",global( "_t"+typeIdent ) ) );
}

#ifdef USE_LLVM
void DeleteEachNode::translate2( Codegen_LLVM *g ){
	llvm::Type *void_ty=llvm::Type::getVoidTy( *g->context );
	auto objty=g->builder->CreateBitOrPointerCast( type->structType()->objty,llvm::PointerType::get( g->bbType,0 ) );
	g->CallIntrinsic( "_bbObjDeleteEach",void_ty,1,objty );
}
#endif
