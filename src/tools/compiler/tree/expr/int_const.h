#ifndef INT_CONST_NODE_H
#define INT_CONST_NODE_H

#include "const.h"

struct IntConstNode : public ConstNode{
	int value;
	IntConstNode( int n );
	TNode *translate( Codegen *g );
	int intValue();
	float floatValue();
	std::string stringValue();
	json toJSON( Environ *e );
#ifdef USE_LLVM
	llvm::Value *translate2( Codegen_LLVM *g );
#endif
};

#endif
