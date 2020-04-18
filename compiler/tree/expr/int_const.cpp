#include "int_const.h"

//////////////////////
// Integer constant //
//////////////////////
IntConstNode::IntConstNode( int n ):value(n){
	sem_type=Type::int_type;
}

TNode *IntConstNode::translate( Codegen *g ){
	return d_new TNode( IR_CONST,0,0,value );
}

int IntConstNode::intValue(){
	return value;
}

float IntConstNode::floatValue(){
	return value;
}

string IntConstNode::stringValue(){
	return itoa( value );
}
