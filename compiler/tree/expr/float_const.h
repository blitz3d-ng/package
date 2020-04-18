#ifndef FLOAT_CONST_NODE_H
#define FLOAT_CONST_NODE_H

#include "const.h"

struct FloatConstNode : public ConstNode{
	float value;
	FloatConstNode( float f );
	TNode *translate( Codegen *g );
	int intValue();
	float floatValue();
	string stringValue();

	json toJSON( Environ *e ){
		json tree;tree["@class"]="FloatConstNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["value"]=value;
		return tree;
	}
};

#endif
