#include "rel_expr.h"
#include "int_const.h"
#include "float_const.h"
#include "string_const.h"

/////////////////////////
// relation expression //
/////////////////////////
ExprNode *RelExprNode::semant( Environ *e ){
	lhs=lhs->semant(e);
	rhs=rhs->semant(e);
	if( lhs->sem_type->structType() || rhs->sem_type->structType() ){
		if( op!='=' && op!=NE ) ex( "Illegal operator for custom type objects" );
		opType=lhs->sem_type!=Type::null_type ? lhs->sem_type : rhs->sem_type;
	}else if( lhs->sem_type==Type::string_type || rhs->sem_type==Type::string_type ){
		opType=Type::string_type;
	}else if( lhs->sem_type==Type::float_type || rhs->sem_type==Type::float_type ){
		opType=Type::float_type;
	}else{
		opType=Type::int_type;
	}
	sem_type=Type::int_type;
	lhs=lhs->castTo( opType,e );
	rhs=rhs->castTo( opType,e );
	ConstNode *lc=lhs->constNode(),*rc=rhs->constNode();
	if( lc && rc ){
		ExprNode *expr;
		if( opType==Type::string_type ){
			switch( op ){
			case '<':expr=d_new IntConstNode( lc->stringValue()< rc->stringValue() );break;
			case '=':expr=d_new IntConstNode( lc->stringValue()==rc->stringValue() );break;
			case '>':expr=d_new IntConstNode( lc->stringValue()> rc->stringValue() );break;
			case LE: expr=d_new IntConstNode( lc->stringValue()<=rc->stringValue() );break;
			case NE: expr=d_new IntConstNode( lc->stringValue()!=rc->stringValue() );break;
			case GE: expr=d_new IntConstNode( lc->stringValue()>=rc->stringValue() );break;
			}
		}else if( opType==Type::float_type ){
			switch( op ){
			case '<':expr=d_new IntConstNode( lc->floatValue()< rc->floatValue() );break;
			case '=':expr=d_new IntConstNode( lc->floatValue()==rc->floatValue() );break;
			case '>':expr=d_new IntConstNode( lc->floatValue()> rc->floatValue() );break;
			case LE: expr=d_new IntConstNode( lc->floatValue()<=rc->floatValue() );break;
			case NE: expr=d_new IntConstNode( lc->floatValue()!=rc->floatValue() );break;
			case GE: expr=d_new IntConstNode( lc->floatValue()>=rc->floatValue() );break;
			}
		}else{
			switch( op ){
			case '<':expr=d_new IntConstNode( lc->intValue()< rc->intValue() );break;
			case '=':expr=d_new IntConstNode( lc->intValue()==rc->intValue() );break;
			case '>':expr=d_new IntConstNode( lc->intValue()> rc->intValue() );break;
			case LE: expr=d_new IntConstNode( lc->intValue()<=rc->intValue() );break;
			case NE: expr=d_new IntConstNode( lc->intValue()!=rc->intValue() );break;
			case GE: expr=d_new IntConstNode( lc->intValue()>=rc->intValue() );break;
			}
		}
		delete this;
		return expr;
	}
	return this;
}

TNode *RelExprNode::translate( Codegen *g ){
	TNode *l=lhs->translate( g );
	TNode *r=rhs->translate( g );
	return compare( op,l,r,opType );
}

#ifdef USE_LLVM
llvm::Value *RelExprNode::translate2( Codegen_LLVM *g ){
	auto *l=lhs->translate2( g );
	auto *r=rhs->translate2( g );

	auto *t=compare2( op,l,r,opType,g );
	return g->builder->CreateIntCast( t,Type::int_type->llvmType( g->context.get() ),true );
}
#endif

json RelExprNode::toJSON( Environ *e ){
	json tree;tree["@class"]="RelExprNode";
	tree["sem_type"]=sem_type->toJSON();
	switch( op ){
	case '<': tree["op"]="LT";break;
	case '=': tree["op"]="EQ";break;
	case '>': tree["op"]="GT";break;
	case LE: tree["op"]="LE";break;
	case NE: tree["op"]="NE";break;
	case GE: tree["op"]="GE";break;
	}
	tree["lhs"]=lhs->toJSON( e );
	tree["rhs"]=rhs->toJSON( e );
	tree["opType"]=opType->toJSON();
	return tree;
}
