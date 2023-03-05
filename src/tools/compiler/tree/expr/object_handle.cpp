#include "object_handle.h"

///////////////////
// Object Handle //
///////////////////
ExprNode *ObjectHandleNode::semant( Environ *e ){
	expr=expr->semant( e );
	if( !expr->sem_type->structType() ) ex( "'ObjectHandle' must be used with an object" );
	sem_type=Type::int_type;
	return this;
}

TNode *ObjectHandleNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	return call( "__bbObjToHandle",t );
}

#ifdef USE_LLVM
llvm::Value *ObjectHandleNode::translate2( Codegen_LLVM *g ){
	auto ty=llvm::Type::getInt64Ty( *g->context );
	auto t=g->CastToObjPtr( expr->translate2( g ) );
	return g->CallIntrinsic( "_bbObjToHandle",ty,1,t );
}
#endif
