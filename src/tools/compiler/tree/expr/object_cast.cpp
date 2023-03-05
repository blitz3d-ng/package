#include "object_cast.h"

/////////////////
// Object cast //
/////////////////
ExprNode *ObjectCastNode::semant( Environ *e ){
	expr=expr->semant( e );
	expr=expr->castTo( Type::int_type,e );
	sem_type=e->findType( type_ident );
	if( !sem_type ) ex( "custom type name not found" );
	if( !sem_type->structType() ) ex( "type is not a custom type" );
	return this;
}

TNode *ObjectCastNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	t=call( "__bbObjFromHandle",t,global( "_t"+sem_type->structType()->ident ) );
	return t;
}

#ifdef USE_LLVM
llvm::Value *ObjectCastNode::translate2( Codegen_LLVM *g ){
	auto ty=llvm::PointerType::get( g->bbObj,0 );
	auto objty=g->builder->CreateBitOrPointerCast( sem_type->structType()->objty,llvm::PointerType::get( g->bbObjType,0 ) );
	auto t=expr->translate2( g );
	t=g->CallIntrinsic( "_bbObjFromHandle",ty,2,t,objty );
	return g->builder->CreateBitOrPointerCast( t,llvm::PointerType::get( sem_type->structType()->structtype,0 ) );
}
#endif
