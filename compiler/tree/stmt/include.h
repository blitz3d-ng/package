#ifndef INCLUDE_NODE_H
#define INCLUDE_NODE_H

#include "node.h"

struct IncludeNode : public StmtNode{
	string file,label;
	StmtSeqNode *stmts;
	IncludeNode( const string &t,StmtSeqNode *ss ):file(t),stmts(ss){}
	~IncludeNode(){ delete stmts; }

	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="IncludeNode";
		tree["pos"]=pos;
		tree["file"]=file;
		tree["label"]=label;
		tree["stmts"]=stmts->toJSON( e );
		return tree;
	}
};

#endif
