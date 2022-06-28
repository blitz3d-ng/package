#include "decl_var.h"

//////////////////
// Declared var //
//////////////////

void DeclVarNode::semant( Environ *e ){
}

TNode *DeclVarNode::translate( Codegen *g ){
	if( sem_decl->kind==DECL_GLOBAL ) return global( "_v"+sem_decl->name );
	return local( sem_decl->offset );
}

#ifdef USE_LLVM
llvm::Value *DeclVarNode::load2( Codegen_LLVM *g ){
	llvm::Value *t=translate2( g );
	if( sem_type==Type::string_type ) return g->CallIntrinsic( "_bbStrLoad",sem_type->llvmType( g->context.get() ),1,t );
	return g->builder->CreateLoad( sem_decl->type->llvmType( g->context.get() ),sem_decl->ptr );
}

llvm::Value *DeclVarNode::translate2( Codegen_LLVM *g ){
	return sem_decl->ptr;
}
#endif

TNode *DeclVarNode::store( Codegen *g,TNode *n ){
	if( isObjParam() ){
		TNode *t=translate( g );
		return move( n,mem( t ) );
	}
	return VarNode::store( g,n );
}

bool DeclVarNode::isObjParam(){
	return sem_type->structType() && sem_decl->kind==DECL_PARAM;
}

json DeclVarNode::toJSON( Environ *e ){
	json tree;tree["@class"]="DeclVarNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["sem_decl"]=sem_decl->toJSON();
	return tree;
}
