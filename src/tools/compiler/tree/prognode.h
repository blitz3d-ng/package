
#ifndef PROGNODE_H
#define PROGNODE_H

#include "decl/decl_seq.h"
#include "stmt/stmt_seq.h"
#include "../codegen.h"

#ifdef USE_LLVM
#include "../codegen_llvm/codegen_llvm.h"
#endif

#include <vector>

extern std::vector<std::string> modules;

struct UserFunc{
	std::string ident,proc,lib;
	UserFunc( const UserFunc &t ):ident(t.ident),proc(t.proc),lib(t.lib){}
	UserFunc( const std::string &id,const std::string &pr,const std::string &lb ):ident(id),proc(pr),lib(lb){}
};

struct ProgNode : public Node{

	DeclSeqNode *consts;
	DeclSeqNode *structs;
	DeclSeqNode *funcs;
	DeclSeqNode *datas;
	StmtSeqNode *stmts;

	Environ *sem_env;

	bool debug;

	std::string file_lab;

	ProgNode( DeclSeqNode *c,DeclSeqNode *s,DeclSeqNode *f,DeclSeqNode *d,StmtSeqNode *ss ):consts(c),structs(s),funcs(f),datas(d),stmts(ss),debug(false){}
	~ProgNode(){
		delete consts;
		delete structs;
		delete funcs;
		delete datas;
		delete stmts;
	}

	Environ *semant( Environ *e );
	void translate( Codegen *g,const std::vector<UserFunc> &userfuncs );

	json toJSON( Environ *e );
	json toJSON( bool dbg );

#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g,const std::vector<UserFunc> &userfuncs );
#endif
};

#endif
