#ifndef VAR_EXPR_NODE_H
#define VAR_EXPR_NODE_H

#include "node.h"
#include "../var/node.h"

struct VarExprNode : public ExprNode{
	VarNode *var;
	VarExprNode( VarNode *v ):var(v){}
	~VarExprNode(){ delete var; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="VarExprNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["var"]=var->toJSON( e );
		return tree;
	}
};

#endif
