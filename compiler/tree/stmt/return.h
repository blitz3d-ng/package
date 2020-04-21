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
	json toJSON( Environ *e );
#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
