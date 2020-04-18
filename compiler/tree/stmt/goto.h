#ifndef GOTONODE_H
#define GOTONODE_H

#include "node.h"

struct GotoNode : public StmtNode{
	string ident;
	GotoNode( const string &s ):ident(s){}
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="GotoNode";
		tree["pos"]=pos;
		tree["ident"]=ident;
		return tree;
	}
};

#endif
