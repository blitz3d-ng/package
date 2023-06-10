#ifndef EXPR_SEQ_NODE
#define EXPR_SEQ_NODE

#include "node.h"

struct ExprSeqNode : public Node{
	std::vector<ExprNode*> exprs;
	~ExprSeqNode(){ for( ;exprs.size();exprs.pop_back() ) delete exprs.back(); }
	void push_back( ExprNode *e ){ exprs.push_back( e ); }
	int  size(){ return exprs.size(); }
	void semant( Environ *e );
	TNode *translate( Codegen *g,bool userlib );
	void castTo( DeclSeq *ds,Environ *e,bool userlib );
	void castTo( Type *t,Environ *e );

	json toJSON( Environ *e ){
		json tree=json::array();
		for( int i=0;i<exprs.size();i++ ){
			tree.push_back( exprs[i]->toJSON( e ) );
		}
		return tree;
	}
};

#endif
