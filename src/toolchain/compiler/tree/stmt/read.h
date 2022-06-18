#ifndef READ_NODE_H
#define READ_NODE_H

#include "node.h"
#include "../var/node.h"

struct ReadNode : public StmtNode{
	VarNode *var;
	ReadNode( VarNode *v ):var(v){}
	~ReadNode(){ delete var; }
	void semant( Environ *e );
	void translate( Codegen *g );
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="ReadNode";
		tree["pos"]=pos;
		tree["var"]=var->toJSON( e );
		return tree;
	}
};

#endif
