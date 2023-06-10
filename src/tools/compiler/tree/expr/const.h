#ifndef CONST_NODE_H
#define CONST_NODE_H

#include "node.h"

struct ConstNode : public ExprNode{
	ExprNode *semant( Environ *e ){ return this; }
	ConstNode *constNode(){ return this; }
	virtual int intValue()=0;
	virtual float floatValue()=0;
	virtual std::string stringValue()=0;

	DEFAULT_NODE_JSON( ConstNode );
};

#endif
