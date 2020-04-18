#ifndef VAR_NODE_H
#define VAR_NODE_H

#include "../node.h"

struct VarNode : public Node{
	Type *sem_type;

	//get set var
	TNode *load( Codegen *g );
	virtual TNode *store( Codegen *g,TNode *n );
	virtual bool isObjParam();

	//addr of var
	virtual void semant( Environ *e )=0;
	virtual TNode *translate( Codegen *g )=0;
};

#endif
