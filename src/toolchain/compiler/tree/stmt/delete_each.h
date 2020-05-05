#ifndef DELETEEACHNODE_H
#define DELETEEACHNODE_H

#include "node.h"

struct DeleteEachNode : public StmtNode{
	string typeIdent;
	DeleteEachNode( const string &t ):typeIdent(t){}
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="DeleteEachNode";
		tree["pos"]=pos;
		tree["typeIdent"]=typeIdent;
		return tree;
	}
};

#endif
