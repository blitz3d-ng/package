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

#ifdef USE_LLVM
llvm::Value *IntConstNode::translate2( Codegen_LLVM *g ){
	return llvm::ConstantInt::get( sem_type->llvmType( g->context.get() ),value,true );
}
#endif

int IntConstNode::intValue(){
	return value;
}

float IntConstNode::floatValue(){
	return value;
}

string IntConstNode::stringValue(){
	return itoa( value );
}

json IntConstNode::toJSON( Environ *e ){
	json tree;tree["@class"]="IntConstNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["value"]=value;
	return tree;
}
