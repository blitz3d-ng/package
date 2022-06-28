#ifndef INSERTNODE_H
#define INSERTNODE_H

#include "node.h"
#include "../expr/node.h"

struct InsertNode : public StmtNode{
	ExprNode *expr1,*expr2;
	bool before;
	InsertNode( ExprNode *e1,ExprNode *e2,bool b ):expr1(e1),expr2(e2),before(b){}
	~InsertNode(){ delete expr1;delete expr2; }
	void semant( Environ *e );
	void translate( Codegen *g );
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="InsertNode";
		tree["pos"]=pos;
		tree["expr1"]=expr1->toJSON( e );
		tree["expr2"]=expr2->toJSON( e );
		tree["before"]=before;
		return tree;
	}
};

#endif
