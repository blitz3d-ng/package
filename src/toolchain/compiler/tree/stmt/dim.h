#ifndef DIMNODE_H
#define DIMNODE_H

#include "node.h"
#include "../expr/expr_seq.h"

struct DimNode : public StmtNode{
	string ident,tag;
	ExprSeqNode *exprs;
	ArrayType *sem_type;
	Decl *sem_decl;
	DimNode( const string &i,const string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~DimNode(){ delete exprs; }
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="DimNode";
		tree["pos"]=pos;
		tree["ident"]=ident;
		tree["tag"]=tag;
		tree["exprs"]=exprs->toJSON( e );
		tree["sem_type"]=sem_type->toJSON();
		tree["sem_decl"]=sem_decl->toJSON();
		return tree;
	}
};

#endif
