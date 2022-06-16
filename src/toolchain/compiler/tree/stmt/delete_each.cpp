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
	auto ty=Type::void_type->llvmType( g->context.get() );
	g->CallIntrinsic( "_bbObjDeleteEach",ty,1,type->structType()->objty );
}
#endif
