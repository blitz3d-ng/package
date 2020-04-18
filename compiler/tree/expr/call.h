#ifndef CALL_NODE_H
#define CALL_NODE_H

#include "node.h"
#include "expr_seq.h"

struct CallNode : public ExprNode{
	string ident,tag;
	ExprSeqNode *exprs;
	Decl *sem_decl;
	CallNode( const string &i,const string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~CallNode(){ delete exprs; }
	ExprNode *semant( Environ *e );
	TNode *translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="CallNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["ident"]=ident;
		tree["tag"]=tag;
		tree["sem_decl"]=sem_decl->toJSON();
		tree["exprs"]=exprs->toJSON( e );
		return tree;
	}
};

#endif
