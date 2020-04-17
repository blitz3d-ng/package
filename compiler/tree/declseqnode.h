#ifndef DECLSEQNODE_H
#define DECLSEQNODE_H

#include "declnode.h"

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

#endif
