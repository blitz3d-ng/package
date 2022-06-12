#include "uni_expr.h"
#include "int_const.h"
#include "float_const.h"
#include "string_const.h"

////////////////////
// Unary operator //
////////////////////
ExprNode *UniExprNode::semant( Environ *e ){
	expr=expr->semant( e );
	sem_type=expr->sem_type;
	if( sem_type!=Type::int_type && sem_type!=Type::float_type ) ex( "Illegal operator for type" );
	if( ConstNode *c=expr->constNode() ){
		ExprNode *e;
		if( sem_type==Type::int_type ){
			switch( op ){
			case '+':e=d_new IntConstNode( +c->intValue() );break;
			case '-':e=d_new IntConstNode( -c->intValue() );break;
			case ABS:e=d_new IntConstNode( c->intValue()>=0 ? c->intValue() : -c->intValue() );break;
			case SGN:e=d_new IntConstNode( c->intValue()>0 ? 1 : (c->intValue()<0 ? -1 : 0) );break;
			}
		}else{
			switch( op ){
			case '+':e=d_new FloatConstNode( +c->floatValue() );break;
			case '-':e=d_new FloatConstNode( -c->floatValue() );break;
			case ABS:e=d_new FloatConstNode( c->floatValue()>=0 ? c->floatValue() : -c->floatValue() );break;
			case SGN:e=d_new FloatConstNode( c->floatValue()>0 ? 1 : (c->floatValue()<0 ? -1 : 0) );break;
			}
		}
		delete this;
		return e;
	}
	return this;
}

TNode *UniExprNode::translate( Codegen *g ){
	int n=0;
	TNode *l=expr->translate( g );
	if( sem_type==Type::int_type ){
		switch( op ){
		case '+':return l;
		case '-':n=IR_NEG;break;
		case ABS:return call( "__bbAbs",l );
		case SGN:return call( "__bbSgn",l );
		}
	}else{
		switch( op ){
		case '+':return l;
		case '-':n=IR_FNEG;break;
		case ABS:return fcall( "__bbFAbs",l );
		case SGN:return fcall( "__bbFSgn",l );
		}
	}
	return d_new TNode( n,l,0 );
}

#ifdef USE_LLVM
llvm::Value *UniExprNode::translate2( Codegen_LLVM *g ){
	auto *l=expr->translate2( g );
	if( sem_type==Type::int_type ){
		switch( op ){
		case '+':return l;
		case '-':return g->builder->CreateNeg( l );
		case ABS:return g->CallIntrinsic( "_bbAbs",sem_type->llvmType( g->context.get() ),1,l );
		case SGN:return g->CallIntrinsic( "_bbSgn",sem_type->llvmType( g->context.get() ),1,l );
		}
	}else{
		switch( op ){
		case '+':return l;
		case '-':return g->builder->CreateFNeg( l );
		case ABS:return g->CallIntrinsic( "_bbFAbs",sem_type->llvmType( g->context.get() ),1,l );
		case SGN:return g->CallIntrinsic( "_bbFSgn",sem_type->llvmType( g->context.get() ),1,l );
		}
	}

	return nullptr;
}
#endif
