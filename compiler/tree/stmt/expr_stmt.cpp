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
