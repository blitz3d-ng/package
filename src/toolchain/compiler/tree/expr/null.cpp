#include "null.h"

/////////////////
// Null object //
/////////////////
ExprNode *NullNode::semant( Environ *e ){
	sem_type=Type::null_type;
	return this;
}

TNode *NullNode::translate( Codegen *g ){
	return d_new TNode( IR_CONST,0,0,0 );
}

#ifdef USE_LLVM
llvm::Value *NullNode::translate2( Codegen_LLVM *g ){
	return llvm::ConstantPointerNull::get( llvm::PointerType::get( g->bbObj,0 ) );
}
#endif
