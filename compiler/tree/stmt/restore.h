#ifndef RESTORE_NODE_H
#define RESTORE_NODE_H

#include "node.h"

struct RestoreNode : public StmtNode{
	string ident;
	Label *sem_label;
	RestoreNode( const string &i ):ident(i){}
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="RestoreNode";
		tree["pos"]=pos;
		tree["ident"]=ident;
		tree["sem_label"]=sem_label->toJSON();
		return tree;
	}
};

#endif
