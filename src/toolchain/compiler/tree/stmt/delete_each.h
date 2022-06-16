#ifndef DELETEEACHNODE_H
#define DELETEEACHNODE_H

#include "node.h"

struct DeleteEachNode : public StmtNode{
	string typeIdent;
	Type *type;
	DeleteEachNode( const string &t ):typeIdent(t){}
	void semant( Environ *e );
	void translate( Codegen *g );
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="DeleteEachNode";
		tree["pos"]=pos;
		tree["typeIdent"]=typeIdent;
		return tree;
	}
};

#endif
