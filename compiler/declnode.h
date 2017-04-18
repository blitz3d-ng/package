
#ifndef DECLNODE_H
#define DECLNODE_H

#include "node.h"

struct DeclNode : public Node{
	int pos;
	string file;
	DeclNode():pos(-1){}
	virtual void proto( DeclSeq *d,Environ *e ){}
	virtual void semant( Environ *e ){}
	virtual void translate( Codegen *g ){}
	virtual void transdata( Codegen *g ){}

	DEFAULT_NODE_JSON( DeclNode );
};

struct DeclSeqNode : public Node{
	vector<DeclNode*> decls;
	DeclSeqNode(){}
	~DeclSeqNode(){ for(;decls.size();decls.pop_back())delete decls.back(); }
	void proto( DeclSeq *d,Environ *e );
	void semant( Environ *e );
	void translate( Codegen *g );
	void transdata( Codegen *g );
	void push_back( DeclNode *d ){ decls.push_back( d ); }
	int  size(){ return decls.size(); }

	json toJSON( Environ *e ){
		json tree=json::array();
		for( int i=0;i<decls.size();i++ ){
			tree.push_back( decls[i]->toJSON( e ) );
		}
		return tree;
	}
};

#include "exprnode.h"
#include "stmtnode.h"

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

	json toJSON( Environ *e ){
		json tree;tree["@class"]="VarDeclNode";
		tree["ident"]=ident;
		tree["tag"]=tag;
		tree["kind"]=kind;
		tree["constant"]=constant;
		if( expr ) tree["expr"]=expr->toJSON( e );
		if( sem_var ) tree["sem_var"]=sem_var->toJSON( e );
		return tree;
	}
};

struct FuncDeclNode : public DeclNode{
	string ident,tag;
	DeclSeqNode *params;
	StmtSeqNode *stmts;
	FuncType *sem_type;
	Environ *sem_env;
	FuncDeclNode( const string &i,const string &t,DeclSeqNode *p,StmtSeqNode *ss ):ident(i),tag(t),params(p),stmts(ss){}
	~FuncDeclNode(){ delete params;delete stmts; }
	void proto( DeclSeq *d,Environ *e );
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="FuncDeclNode";
		tree["ident"]=ident;
		tree["tag"]=tag;
		tree["locals"]=json::array();
		for( int k=0;k<sem_env->decls->size();++k ){
			Decl *d=sem_env->decls->decls[k];
			if( d->kind!=DECL_LOCAL ) continue;
			tree["locals"].push_back( d->toJSON() );
		}
		tree["params"]=params->toJSON( sem_env );
		tree["stmts"]=stmts->toJSON( sem_env );
		tree["sem_type"]=sem_type->toJSON( );
		return tree;
	}
};

struct StructDeclNode : public DeclNode{
	string ident;
	DeclSeqNode *fields;
	StructType *sem_type;
	StructDeclNode( const string &i,DeclSeqNode *f ):ident(i),fields(f){}
	~StructDeclNode(){ delete fields; }
	void proto( DeclSeq *d,Environ *e );
	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e ){
		json tree;tree["@class"]="StructDeclNode";
		tree["ident"]=ident;
		tree["fields"]=fields->toJSON( e );
		tree["sem_type"]=sem_type->toJSON();
		return tree;
	}
};

struct DataDeclNode : public DeclNode{
	ExprNode *expr;
	string str_label;
	DataDeclNode( ExprNode *e ):expr(e){}
	~DataDeclNode(){ delete expr; }
	void proto( DeclSeq *d,Environ *e );
	void semant( Environ *e );
	void translate( Codegen *g );
	void transdata( Codegen *g );

	DEFAULT_NODE_JSON( DataDeclNode );
};

struct VectorDeclNode : public DeclNode{
	string ident,tag;
	ExprSeqNode *exprs;
	int kind;
	VectorType *sem_type;
	VectorDeclNode( const string &i,const string &t,ExprSeqNode *e,int k ):ident(i),tag(t),exprs(e),kind(k){}
	~VectorDeclNode(){ delete exprs; }
	void proto( DeclSeq *d,Environ *e );
	void translate( Codegen *g );

	DEFAULT_NODE_JSON( VectorDeclNode );
};

#endif
