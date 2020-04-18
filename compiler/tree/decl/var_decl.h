#ifndef VARDECLNODE_H
#define VARDECLNODE_H

#include "node.h"
#include "../expr/node.h"
#include "../var/decl_var.h"

//'kind' shouldn't really be in Parser...
//should probably be LocalDeclNode,GlobalDeclNode,ParamDeclNode
struct VarDeclNode : public DeclNode{
	string ident,tag;
	int kind;bool constant;
	ExprNode *expr;
	DeclVarNode *sem_var;
	VarDeclNode( const string &i,const string &t,int k,bool c,ExprNode *e ):ident(i),tag(t),kind(k),constant(c),expr(e),sem_var(0){}
	~VarDeclNode(){ delete expr;delete sem_var; }
	void proto( DeclSeq *d,Environ *e );
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e );
};

#endif
