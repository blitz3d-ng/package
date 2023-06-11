#include "float_const.h"

////////////////////
// Float constant //
////////////////////
FloatConstNode::FloatConstNode( float f ):value(f){
	sem_type=Type::float_type;
}

TNode *FloatConstNode::translate( Codegen *g ){
	return d_new TNode( IR_CONST,0,0,*(int*)&value );
}

int FloatConstNode::intValue(){
// TODO: figure out a proper, cross platform implementation
#ifdef BB_WIN32
	float flt=value;
	int temp;
	_control87( _RC_NEAR|_PC_24|_EM_INVALID|_EM_ZERODIVIDE|_EM_OVERFLOW|_EM_UNDERFLOW|_EM_INEXACT|_EM_DENORMAL,0xfffff );
	_asm{
		fld [flt];
		fistp [temp];
	}
	_control87( _CW_DEFAULT,0xfffff );
	return temp;
#else
	return value;
#endif
}

float FloatConstNode::floatValue(){
	return value;
}

std::string FloatConstNode::stringValue(){
	return ftoa( value );
}

#ifdef USE_LLVM
llvm::Value *FloatConstNode::translate2( Codegen_LLVM *g ){
	return llvm::ConstantFP::get( *g->context,llvm::APFloat( (double)value ) );
}
#endif

json FloatConstNode::toJSON( Environ *e ){
	json tree;tree["@class"]="FloatConstNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["value"]=value;
	return tree;
}
