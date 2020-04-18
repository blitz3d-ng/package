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

	json toJSON( Environ *e ){
		json tree;tree["@class"]="ExprStmtNode";
		tree["pos"]=pos;
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

#endif
