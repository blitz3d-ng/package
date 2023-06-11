#ifndef STMT_SEQ_NODE_H
#define STMT_SEQ_NODE_H

#include "node.h"

struct StmtSeqNode : public Node{
	std::string file;
	std::vector<StmtNode*> stmts;
	bool strict;
	StmtSeqNode( const std::string &f ):file(f),strict(false){}
	~StmtSeqNode(){ for( ;stmts.size();stmts.pop_back() ) delete stmts.back(); }
	void semant( Environ *e );
	void translate( Codegen *g );
	void push_back( StmtNode *s ){ stmts.push_back( s ); }
	int  size(){ return stmts.size(); }

	static void reset( const std::string &file,const std::string &lab );

	json toJSON( Environ *e );

#ifdef USE_LLVM
	void translate2( Codegen_LLVM *g );
#endif
};

#endif
