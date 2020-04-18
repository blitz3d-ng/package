#ifndef EXITNODE_H
#define EXITNODE_H

#include "node.h"

struct ExitNode : public StmtNode{
	string sem_brk;
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="ExitNode";
		tree["pos"]=pos;
		tree["sem_brk"]=sem_brk;
		return tree;
	}
};

#endif
