#include "cast.h"
#include "int_const.h"
#include "float_const.h"
#include "string_const.h"

ExprNode *CastNode::semant( Environ *e ){
	if( !expr->sem_type ){
		expr=expr->semant( e );
	}

	if( ConstNode *c=expr->constNode() ){
		ExprNode *e;
		if( type==Type::int_type ) e=d_new IntConstNode( c->intValue() );
		else if( type==Type::float_type ) e=d_new FloatConstNode( c->floatValue() );
		else e=d_new StringConstNode( c->stringValue() );
		delete this;
		return e;
	}

	sem_type=type;
	return this;
}

//////////////////////////////////
// Cast an expression to a type //
//////////////////////////////////
TNode *CastNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	if( expr->sem_type==Type::float_type && sem_type==Type::int_type ){
		//float->int
		return d_new TNode( IR_CAST,t,0 );
	}
	if( expr->sem_type==Type::int_type && sem_type==Type::float_type ){
		//int->float
		return d_new TNode( IR_FCAST,t,0 );
	}
	if( expr->sem_type==Type::string_type && sem_type==Type::int_type ){
		//str->int
		return call( "__bbStrToInt",t );
	}
	if( expr->sem_type==Type::int_type && sem_type==Type::string_type ){
		//int->str
		return call( "__bbStrFromInt",t );
	}
	if( expr->sem_type==Type::string_type && sem_type==Type::float_type ){
		//str->float
		return fcall( "__bbStrToFloat",t );
	}
	if( expr->sem_type==Type::float_type && sem_type==Type::string_type ){
		//float->str
		return call( "__bbStrFromFloat",t );
	}
	if( expr->sem_type->structType() && sem_type==Type::string_type ){
		//obj->str
		return call( "__bbObjToStr",t );
	}
	return t;
}

#ifdef USE_LLVM
llvm::Value *CastNode::translate2( Codegen_LLVM *g ){
	llvm::Value *t=expr->translate2( g );
	if( expr->sem_type==Type::float_type && sem_type==Type::int_type ){
		//float->int
		return g->builder->CreateIntCast( t,Type::int_type->llvmType( g->context.get() ),true );
	}
	if( expr->sem_type==Type::int_type && sem_type==Type::float_type ){
		//int->float
		return g->builder->CreateCast( llvm::Instruction::SIToFP,t,Type::float_type->llvmType( g->context.get() ) );
	}
	if( expr->sem_type==Type::string_type && sem_type==Type::int_type ){
		//str->int
		return g->CallIntrinsic( "_bbStrToInt",Type::int_type->llvmType( g->context.get() ),1,t );
	}
	if( expr->sem_type==Type::int_type && sem_type==Type::string_type ){
		//int->str
		return g->CallIntrinsic( "_bbStrFromInt",Type::string_type->llvmType( g->context.get() ),1,t );
	}
	if( expr->sem_type==Type::string_type && sem_type==Type::float_type ){
		//str->float
		return g->CallIntrinsic( "_bbStrToFloat",Type::float_type->llvmType( g->context.get() ),1,t );
	}
	if( expr->sem_type==Type::float_type && sem_type==Type::string_type ){
		//float->str
		return g->CallIntrinsic( "_bbStrFromFloat",Type::string_type->llvmType( g->context.get() ),1,t );
	}
	if( expr->sem_type->structType() && sem_type==Type::string_type ){
		//obj->str
		return g->CallIntrinsic( "_bbObjToStr",Type::string_type->llvmType( g->context.get() ),1,t );
	}
	return t;
}
#endif

json CastNode::toJSON( Environ *e ){
	json tree;tree["@class"]="CastNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["type"]=type->toJSON();
	tree["expr"]=expr->toJSON( e );
	return tree;
}
