#include "after.h"

////////////////////
// Next of object //
////////////////////
ExprNode *AfterNode::semant( Environ *e ){
	expr=expr->semant( e );
	if( expr->sem_type==Type::null_type ) ex( "'After' cannot be used on 'Null'" );
	if( expr->sem_type->structType()==0 ) ex( "'After' must be used with a custom type object" );
	sem_type=expr->sem_type;
	return this;
}

TNode *AfterNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	if( g->debug ) t=jumpf( t,"__bbNullObjEx" );
	return call( "__bbObjNext",t );
}

#ifdef USE_LLVM
llvm::Value *AfterNode::translate2( Codegen_LLVM *g ){
	auto objty=llvm::PointerType::get( g->bbObj,0 );

	auto t=g->CastToObjPtr( expr->translate2( g ) );
	t=g->CallIntrinsic( "_bbObjNext",objty,1,t );
	return g->builder->CreateBitOrPointerCast( t,llvm::PointerType::get( sem_type->structType()->structtype,0 ) );
}
#endif

json AfterNode::toJSON( Environ *e ){
	json tree;tree["@class"]="AfterNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["expr"]=expr->toJSON( e );
	return tree;
}
