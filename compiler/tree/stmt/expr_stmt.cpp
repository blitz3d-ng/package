#include "expr_stmt.h"

//////////////////////////
// Expression statement //
//////////////////////////
void ExprStmtNode::semant( Environ *e ){
	expr=expr->semant( e );
}

void ExprStmtNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	if( expr->sem_type==Type::string_type ) t=call( "__bbStrRelease",t );
	g->code( t );
}

#ifdef USE_LLVM
void ExprStmtNode::translate2( Codegen_LLVM *g ){
	auto v=expr->translate2( g );
	if( expr->sem_type==Type::string_type ) g->CallIntrinsic( "_bbStrRelease",llvm::Type::getVoidTy( g->context ),1,v );
}
#endif

json ExprStmtNode::toJSON( Environ *e ){
	json tree;tree["@class"]="ExprStmtNode";
	tree["pos"]=pos;
	tree["expr"]=expr->toJSON( e );
	return tree;
}
