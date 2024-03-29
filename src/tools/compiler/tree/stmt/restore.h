#ifndef RESTORE_NODE_H
#define RESTORE_NODE_H

#include "node.h"

struct RestoreNode : public StmtNode{
	std::string ident;
	Label *sem_label;
	RestoreNode( const std::string &i ):ident(i){}
	void semant( Environ *e );
	void translate( Codegen *g );
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="RestoreNode";
		tree["pos"]=pos;
		tree["ident"]=ident;
		if( sem_label ) tree["sem_label"]=sem_label->toJSON();
		return tree;
	}
};

#endif
