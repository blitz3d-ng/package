#ifndef GOSUBNODE_H
#define GOSUBNODE_H

#include "node.h"

struct GosubNode : public StmtNode{
	string ident;
	GosubNode( const string &s ):ident(s){}
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="GosubNode";
		tree["pos"]=pos;
		tree["ident"]=ident;
		return tree;
	}
};

#endif
