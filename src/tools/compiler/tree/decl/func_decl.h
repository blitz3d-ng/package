#ifndef FUNCDECLNODE_H
#define FUNCDECLNODE_H

#include "decl_seq.h"
#include "../stmt/stmt_seq.h"

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
	json toJSON( Environ *e );

#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
