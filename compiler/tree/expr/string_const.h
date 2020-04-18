#ifndef STRING_CONST_NODE_H
#define STRING_CONST_NODE_H

#include "const.h"

struct StringConstNode : public ConstNode{
	string value;
	StringConstNode( const string &s );
	TNode *translate( Codegen *g );
	int intValue();
	float floatValue();
	string stringValue();

	json toJSON( Environ *e ){
		json tree;tree["@class"]="StringConstNode";
		tree["sem_type"]=sem_type->toJSON();
		tree["value"]=value;
		return tree;
	}
};

#endif
