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
	json toJSON( Environ *e );
#ifdef USE_LLVM
	llvm::Value *translate2( Codegen_LLVM *g );
#endif
};

#endif
