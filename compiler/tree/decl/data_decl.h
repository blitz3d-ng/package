#ifndef DATADECLNODE_H
#define DATADECLNODE_H

#include "node.h"
#include "../expr/node.h"

struct DataDeclNode : public DeclNode{
	ExprNode *expr;
	string str_label;
	DataDeclNode( ExprNode *e ):expr(e){}
	~DataDeclNode(){ delete expr; }
	void proto( DeclSeq *d,Environ *e );
	void semant( Environ *e );
	void translate( Codegen *g );
	void transdata( Codegen *g );

	json toJSON( Environ *e );
};

#endif
