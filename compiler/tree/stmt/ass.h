#ifndef ASSNODE_H
#define ASSNODE_H

#include "node.h"
#include "../expr/node.h"
#include "../var/node.h"

struct AssNode : public StmtNode{
	VarNode *var;
	ExprNode *expr;
	AssNode( VarNode *var,ExprNode *expr ):var(var),expr(expr){}
	~AssNode(){ delete var;delete expr; }
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="AssNode";
		tree["pos"]=pos;
		tree["var"]=var->toJSON( e );
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

#endif
