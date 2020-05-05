#ifndef FORNODE_H
#define FORNODE_H

#include "node.h"
#include "../expr/node.h"
#include "../stmt/stmt_seq.h"

struct ForNode : public StmtNode{
	int nextPos;
	VarNode *var;
	ExprNode *fromExpr,*toExpr,*stepExpr;
	StmtSeqNode *stmts;
	string sem_brk;
	ForNode( VarNode *v,ExprNode *f,ExprNode *t,ExprNode *s,StmtSeqNode *ss,int np );
	~ForNode();
	void semant( Environ *e );
	void translate( Codegen *g );
	json toJSON( Environ *e );
};

#endif
