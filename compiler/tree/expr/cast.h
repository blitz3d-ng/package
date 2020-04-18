#ifndef CAST_NODE_H
#define CAST_NODE_H

#include "node.h"

struct CastNode : public ExprNode{
	ExprNode *expr;
	Type *type;
	CastNode( ExprNode *ex,Type *ty ):expr( ex ),type( ty ){}
	~CastNode(){ delete expr; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="CastNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["type"]=type->toJSON();
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

#endif
