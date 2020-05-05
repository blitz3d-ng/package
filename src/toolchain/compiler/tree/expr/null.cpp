#include "null.h"

/////////////////
// Null object //
/////////////////
ExprNode *NullNode::semant( Environ *e ){
	sem_type=Type::null_type;
	return this;
}

TNode *NullNode::translate( Codegen *g ){
	return d_new TNode( IR_CONST,0,0,0 );
}
