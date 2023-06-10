#include "insert.h"

///////////////////////////
// Insert object in list //
///////////////////////////
void InsertNode::semant( Environ *e ){
	expr1=expr1->semant( e );
	expr2=expr2->semant( e );
	StructType *t1=expr1->sem_type->structType();
	StructType *t2=expr2->sem_type->structType();
	if( !t1 || !t2 ) ex( "Illegal expression type" );
	if( t1!=t2 ) ex( "Objects types are differnt" );
}

void InsertNode::translate( Codegen *g ){
	TNode *t1=expr1->translate( g );
	if( g->debug ) t1=jumpf( t1,"__bbNullObjEx" );
	TNode *t2=expr2->translate( g );
	if( g->debug ) t2=jumpf( t2,"__bbNullObjEx" );
	std::string s=before ? "__bbObjInsBefore" : "__bbObjInsAfter";
	g->code( call( s,t1,t2 ) );
}

#ifdef USE_LLVM
void InsertNode::translate2( Codegen_LLVM *g ){
	auto t1=g->CastToObjPtr( expr1->translate2( g ) );
	auto t2=g->CastToObjPtr( expr2->translate2( g ) );
	std::string s=before ? "_bbObjInsBefore" : "_bbObjInsAfter";
	g->CallIntrinsic( s,Type::void_type->llvmType( g->context.get() ),2,t1,t2 );
}
#endif
