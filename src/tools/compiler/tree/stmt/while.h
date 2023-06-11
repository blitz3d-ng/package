#ifndef WHILE_NODE_H
#define WHILE_NODE_H

#include "node.h"
#include "../expr/node.h"
#include "../stmt/stmt_seq.h"

struct WhileNode : public StmtNode{
	int wendPos;
	ExprNode *expr;
	StmtSeqNode *stmts;
	std::string sem_brk;
	WhileNode( ExprNode *e,StmtSeqNode *s,int wp ):expr(e),stmts(s),wendPos(wp){}
	~WhileNode(){ delete expr;delete stmts; }
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e );

#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
