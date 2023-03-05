#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "../node.h"

struct ConstNode;	//is constant int,float or string

struct ExprNode : public Node{
	Type *sem_type;
	ExprNode():sem_type(0){}
	ExprNode( Type *t ):sem_type( t ){}

	ExprNode *castTo( Type *ty,Environ *e );
	ExprNode *semant( Environ *e,Type *ty );

	virtual ExprNode *semant( Environ *e )=0;
	virtual TNode *translate( Codegen *g )=0;
	virtual ConstNode *constNode(){ return 0; }

	DEFAULT_NODE_JSON( ExprNode );

#ifdef USE_LLVM
	virtual llvm::Value *translate2( Codegen_LLVM *g );
#endif
};

#endif
