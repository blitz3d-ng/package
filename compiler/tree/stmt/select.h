#ifndef SELECT_NODE_H
#define SELECT_NODE_H

#include "node.h"
#include "../expr/expr_seq.h"
#include "../stmt/stmt_seq.h"
#include "../var/node.h"

struct CaseNode : public Node{
	ExprSeqNode *exprs;
	StmtSeqNode *stmts;
	CaseNode( ExprSeqNode *e,StmtSeqNode *s ):exprs(e),stmts(s){}
	~CaseNode(){ delete exprs;delete stmts; }

	json toJSON( Environ *e );
};

struct SelectNode : public StmtNode{
	ExprNode *expr;
	StmtSeqNode *defStmts;
	vector<CaseNode*> cases;
	VarNode *sem_temp;
	SelectNode( ExprNode *e ):expr(e),defStmts(0),sem_temp(0){}
	~SelectNode(){ delete expr;delete defStmts;delete sem_temp;for( ;cases.size();cases.pop_back() ) delete cases.back(); }
	void push_back( CaseNode *c ){ cases.push_back( c ); }
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e );
};

#endif
