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

	json toJSON( Environ *e ){
		json tree;tree["@class"]="StmtSeqNode";
		tree["file"]=file;
		tree["stmts"]=json::array();
		for( int k=0;k<stmts.size();++k ){
			tree["stmts"].push_back( stmts[k]->toJSON( e ) );
		}
		return tree;
	}
};

#endif
