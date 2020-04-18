#ifndef IDENT_NODE_H
#define IDENT_NODE_H

#include "decl_var.h"

struct IdentVarNode : public DeclVarNode{
	string ident,tag;
	IdentVarNode( const string &i,const string &t ):ident(i),tag(t){}
	void semant( Environ *e );

	json toJSON( Environ *e );
};

#endif
