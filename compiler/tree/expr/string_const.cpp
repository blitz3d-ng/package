#include "string_const.h"

/////////////////////
// String constant //
/////////////////////
StringConstNode::StringConstNode( const string &s ):value(s){
	sem_type=Type::string_type;
}

TNode *StringConstNode::translate( Codegen *g ){
	string lab=genLabel();
	g->s_data( value,lab );
	return call( "__bbStrConst",global( lab ) );
}

int StringConstNode::intValue(){
	return atoi( value );
}

float StringConstNode::floatValue(){
	return (float)atof( value );
}

string StringConstNode::stringValue(){
	return value;
}
