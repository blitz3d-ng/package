#ifndef DECLSTMTNODE_H
#define DECLSTMTNODE_H

#include "node.h"
#include "../decl/node.h"

struct DeclStmtNode : public StmtNode{
	DeclNode *decl;
	DeclStmtNode( DeclNode *d ):decl(d){ pos=d->pos; }
	~DeclStmtNode(){ delete decl; }
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="DeclStmtNode";
		tree["pos"]=pos;
		tree["decl"]=decl->toJSON( e );
		return tree;
	}
};

#endif
