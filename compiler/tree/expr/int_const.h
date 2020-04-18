#ifndef INT_CONST_NODE_H
#define INT_CONST_NODE_H

#include "const.h"

struct IntConstNode : public ConstNode{
	int value;
	IntConstNode( int n );
	TNode *translate( Codegen *g );
	int intValue();
	float floatValue();
	string stringValue();

	json toJSON( Environ *e ){
		json tree;tree["@class"]="IntConstNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["value"]=value;
		return tree;
	}
};

#endif
