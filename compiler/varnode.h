
#ifndef VARNODE_H
#define VARNODE_H

#include "varnode.h"

struct VarNode : public Node{
	Type *sem_type;

	//get set var
	TNode *load( Codegen *g );
	virtual TNode *store( Codegen *g,TNode *n );
	virtual bool isObjParam();

	//addr of var
	virtual void semant( Environ *e )=0;
	virtual TNode *translate( Codegen *g )=0;
};

#include "decl.h"

struct DeclVarNode : public VarNode{
	Decl *sem_decl;
	DeclVarNode( Decl *d=0 ):sem_decl(d){ if( d ) sem_type=d->type; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );
	virtual TNode *store( Codegen *g,TNode *n );
	bool isObjParam();
};

struct IdentVarNode : public DeclVarNode{
	string ident,tag;
	IdentVarNode( const string &i,const string &t ):ident(i),tag(t){}
	void semant( Environ *e );

	json toJSON(){
		json tree;tree["kind"]="IdentVarNode";
		tree["ident"]=ident;
		tree["tag"]=tag;
		return tree;
	}
};

struct ArrayVarNode : public VarNode{
	string ident,tag;
	ExprSeqNode *exprs;
	Decl *sem_decl;
	ArrayVarNode( const string &i,const string &t,ExprSeqNode *e ):ident(i),tag(t),exprs(e){}
	~ArrayVarNode(){ delete exprs; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );

	DEFAULT_NODE_JSON( ArrayVarNode );
};

struct FieldVarNode : public VarNode{
	ExprNode *expr;
	string ident,tag;
	Decl *sem_field;
	FieldVarNode( ExprNode *e,const string &i,const string &t ):expr(e),ident(i),tag(t){}
	~FieldVarNode(){ delete expr; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );

	DEFAULT_NODE_JSON( FieldVarNode );
};

struct VectorVarNode : public VarNode{
	ExprNode *expr;
	ExprSeqNode *exprs;
	VectorType *vec_type;
	VectorVarNode( ExprNode *e,ExprSeqNode *es ):expr(e),exprs(es){}
	~VectorVarNode(){ delete expr;delete exprs; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );

	DEFAULT_NODE_JSON( VectorVarNode );
};

#endif
