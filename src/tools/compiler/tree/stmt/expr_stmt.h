#ifndef EXPR_STMT_NODE_H
#define EXPR_STMT_NODE_H

#include "node.h"
#include "../expr/node.h"

struct ExprStmtNode : public StmtNode{
	ExprNode *expr;
	ExprStmtNode( ExprNode *e ):expr(e){}
	~ExprStmtNode(){ delete expr; }
	void semant( Environ *e );
	void translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
