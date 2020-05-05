#ifndef LABEL_NODE_H
#define LABEL_NODE_H

#include "node.h"

struct LabelNode : public StmtNode{
	string ident;
	int data_sz;
	LabelNode( const string &s,int sz ):ident(s),data_sz(sz){}
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="LabelNode";
		tree["pos"]=pos;
		tree["ident"]=ident;
		tree["data_sz"]=data_sz;
		return tree;
	}
};

#endif
