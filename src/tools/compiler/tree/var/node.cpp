#include "node.h"

//////////////////////////////////
// Common get/set for variables //
//////////////////////////////////
TNode *VarNode::load( Codegen *g ){
	TNode *t=translate( g );
	if( sem_type==Type::string_type ) return call( "__bbStrLoad",t );
	return mem( t );
}

#ifdef USE_LLVM
llvm::Value *VarNode::load2( Codegen_LLVM *g ){
	auto t=translate2( g );
	if( sem_type==Type::string_type ) return g->CallIntrinsic( "_bbStrLoad",sem_type->llvmType( g->context.get() ),1,t );
	return g->builder->CreateLoad( sem_type->llvmType( g->context.get() ),t );
}
#endif

TNode *VarNode::store( Codegen *g,TNode *n ){
	TNode *t=translate( g );
	if( sem_type->structType() ) return call( "__bbObjStore",t,n );
	if( sem_type==Type::string_type ) return call( "__bbStrStore",t,n );
	return move( n,mem( t ) );
}

#ifdef USE_LLVM
void VarNode::store2( Codegen_LLVM *g,llvm::Value *v ){
	llvm::Type *void_ty=llvm::Type::getVoidTy( *g->context );

	auto t=translate2( g );
	if( sem_type->structType() ){
		auto ptrptr=llvm::PointerType::get( llvm::PointerType::get( g->bbObj,0 ),0 );
		t=g->builder->CreateBitOrPointerCast( t,ptrptr );
		v=g->CastToObjPtr( v );
		g->CallIntrinsic( "_bbObjStore",void_ty,2,t,v );
	}else if( sem_type==Type::string_type ){
		g->CallIntrinsic( "_bbStrStore",void_ty,2,t,v );
	} else {
		g->builder->CreateStore( v,t );
	}
}
#endif

bool VarNode::isObjParam(){
	return false;
}

#ifdef USE_LLVM
llvm::Value *VarNode::translate2( Codegen_LLVM *g ){
	std::cerr<<"translate2 missing implementation for "<<typeid(*this).name()<<std::endl;
	abort();
}
#endif
