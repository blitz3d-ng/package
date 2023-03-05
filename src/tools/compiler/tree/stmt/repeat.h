#ifndef REPEAT_NODE_H
#define REPEAT_NODE_H

#include "node.h"
#include "../stmt/stmt_seq.h"
#include "../expr/node.h"

struct RepeatNode : public StmtNode{
	int untilPos;
	StmtSeqNode *stmts;
	ExprNode *expr;
	string sem_brk;
	RepeatNode( StmtSeqNode *s,ExprNode *e,int up ):stmts(s),expr(e),untilPos(up){}
	~RepeatNode(){ delete stmts;delete expr; }
	void semant( Environ *e );
	void translate( Codegen *g );
	json toJSON( Environ *e );
#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
