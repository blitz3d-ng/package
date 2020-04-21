#ifndef INCLUDE_NODE_H
#define INCLUDE_NODE_H

#include "node.h"
#include "../stmt/stmt_seq.h"

struct IncludeNode : public StmtNode{
	string file,label;
	StmtSeqNode *stmts;
	IncludeNode( const string &t,StmtSeqNode *ss ):file(t),stmts(ss){}
	~IncludeNode(){ delete stmts; }

	void semant( Environ *e );
	void translate( Codegen *g );

	json toJSON( Environ *e );
#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
