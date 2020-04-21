#ifndef STMT_SEQ_NODE_H
#define STMT_SEQ_NODE_H

#include "node.h"

struct StmtSeqNode : public Node{
	string file;
	vector<StmtNode*> stmts;
	StmtSeqNode( const string &f ):file(f){}
	~StmtSeqNode(){ for( ;stmts.size();stmts.pop_back() ) delete stmts.back(); }
	void semant( Environ *e );
	void translate( Codegen *g );
	void push_back( StmtNode *s ){ stmts.push_back( s ); }
	int  size(){ return stmts.size(); }

	static void reset( const string &file,const string &lab );

	json toJSON( Environ *e );

#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
