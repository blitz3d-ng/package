#ifndef AFTER_NODE_H
#define AFTER_NODE_H

#include "node.h"

struct AfterNode : public ExprNode{
	ExprNode *expr;
	AfterNode( ExprNode *e ):expr(e){}
	~AfterNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e );
};

#endif
