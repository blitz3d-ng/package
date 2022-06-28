#ifndef FIELD_VAR_NODE_H
#define FIELD_VAR_NODE_H

#include "node.h"
#include "../expr/node.h"

struct FieldVarNode : public VarNode{
	ExprNode *expr;
	string ident,tag;
	Decl *sem_field;
	FieldVarNode( ExprNode *e,const string &i,const string &t ):expr(e),ident(i),tag(t){}
	~FieldVarNode(){ delete expr; }
	void semant( Environ *e );
	TNode *translate( Codegen *g );
#ifdef USE_LLVM
	virtual llvm::Value *translate2( Codegen_LLVM *g );
#endif

	json toJSON( Environ *e );
};

#endif
