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
	return g->builder->CreateLoad( sem_type->llvmType( g->context.get() ),translate2( g ) );
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
	g->builder->CreateStore( v,translate2( g ) );
}
#endif

bool VarNode::isObjParam(){
	return false;
}

#ifdef USE_LLVM
llvm::Value *VarNode::translate2( Codegen_LLVM *g ){
	cerr<<"translate2 missing implementation for "<<typeid(*this).name()<<endl;
	abort();
}
#endif
