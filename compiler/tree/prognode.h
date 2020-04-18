
#ifndef PROGNODE_H
#define PROGNODE_H

#include "decl/decl_seq.h"
#include "stmt/stmt_seq.h"
#include "../codegen.h"

#include <vector>

extern vector<string> modules;

struct UserFunc{
	string ident,proc,lib;
	UserFunc( const UserFunc &t ):ident(t.ident),proc(t.proc),lib(t.lib){}
	UserFunc( const string &id,const string &pr,const string &lb ):ident(id),proc(pr),lib(lb){}
};

struct ProgNode : public Node{

	DeclSeqNode *consts;
	DeclSeqNode *structs;
	DeclSeqNode *funcs;
	DeclSeqNode *datas;
	StmtSeqNode *stmts;

	Environ *sem_env;

	bool debug;

	string file_lab;

	ProgNode( DeclSeqNode *c,DeclSeqNode *s,DeclSeqNode *f,DeclSeqNode *d,StmtSeqNode *ss ):consts(c),structs(s),funcs(f),datas(d),stmts(ss),debug(false){}
	~ProgNode(){
		delete consts;
		delete structs;
		delete funcs;
		delete datas;
		delete stmts;
	}

	Environ *semant( Environ *e );
	void translate( Codegen *g,const vector<UserFunc> &userfuncs );

	json toJSON( Environ *e );
	json toJSON( bool dbg );
};

#endif
