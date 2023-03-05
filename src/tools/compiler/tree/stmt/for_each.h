#ifndef FOR_EACH_NODE_H
#define FOR_EACH_NODE_H

#include "node.h"
#include "../stmt/stmt_seq.h"
#include "../var/node.h"

struct ForEachNode : public StmtNode{
	int nextPos;
	VarNode *var;
	string typeIdent;
	StmtSeqNode *stmts;
	string sem_brk;
	ForEachNode( VarNode *v,const string &t,StmtSeqNode *s,int np):var(v),typeIdent(t),stmts(s),nextPos(np){}
	~ForEachNode(){ delete var;delete stmts; }
	void semant( Environ *e );
	void translate( Codegen *g );
#ifdef USE_LLVM
	virtual void translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e ){
		json tree;tree["@class"]="ForEachNode";
		tree["pos"]=pos;
		tree["nextPos"]=nextPos;
		tree["var"]=var->toJSON( e );
		tree["typeIdent"]=typeIdent;
		tree["stmts"]=stmts->toJSON( e );
		tree["sem_brk"]=sem_brk;
		return tree;
	}
};

#endif
