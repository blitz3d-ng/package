#ifndef RETURN_NODE_H
#define RETURN_NODE_H

#include "node.h"
#include "../expr/node.h"

struct ReturnNode : public StmtNode{
	ExprNode *expr;
	string returnLabel;
	ReturnNode( ExprNode *e ):expr( e ){}
	~ReturnNode(){ delete expr; }
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="ReturnNode";
		tree["pos"]=pos;
		if( expr ) tree["expr"]=expr->toJSON( e );
		tree["returnLabel"]=returnLabel;
		return tree;
	}
};

#endif
