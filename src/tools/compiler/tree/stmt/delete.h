#ifndef DELETENODE_H
#define DELETENODE_H

#include "node.h"
#include "../expr/node.h"

struct DeleteNode : public StmtNode{
	ExprNode *expr;
	DeleteNode( ExprNode *e ):expr(e){}
	~DeleteNode(){ delete expr; }
	void semant( Environ *e );
	void translate( Codegen *g );
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="DeleteNode";
		tree["pos"]=pos;
		tree["expr"]=expr->toJSON( e );
		return tree;
	}
};

#endif
