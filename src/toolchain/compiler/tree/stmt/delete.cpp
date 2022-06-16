#include "delete.h"

//////////////////////
// Delete statement //
//////////////////////
void DeleteNode::semant( Environ *e ){
	expr=expr->semant( e );
	if( expr->sem_type->structType()==0 ) ex( "Can't delete non-Newtype" );
}

void DeleteNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	g->code( call( "__bbObjDelete",t ) );
}

#ifdef USE_LLVM
void DeleteNode::translate2( Codegen_LLVM *g ){
	auto ty=Type::void_type->llvmType( g->context.get() );
	auto t=g->CastToObjPtr( expr->translate2( g ) );
	g->CallIntrinsic( "_bbObjDelete",ty,1,t );
}
#endif
