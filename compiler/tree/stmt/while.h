#ifndef WHILE_NODE_H
#define WHILE_NODE_H

#include "node.h"
#include "../expr/node.h"
#include "../stmt/stmt_seq.h"

struct WhileNode : public StmtNode{
	int wendPos;
	ExprNode *expr;
	StmtSeqNode *stmts;
	string sem_brk;
	WhileNode( ExprNode *e,StmtSeqNode *s,int wp ):expr(e),stmts(s),wendPos(wp){}
	~WhileNode(){ delete expr;delete stmts; }
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="WhileNode";
		tree["pos"]=pos;
		tree["wendPos"]=wendPos;
		tree["expr"]=expr->toJSON( e );
		tree["stmts"]=stmts->toJSON( e );
		tree["sem_brk"]=sem_brk;
		return tree;
	}
};

#endif
