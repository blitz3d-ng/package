#include "arith_expr.h"
#include "int_const.h"
#include "float_const.h"
#include "string_const.h"

///////////////////////////
// arithmetic expression //
///////////////////////////
ExprNode *ArithExprNode::semant( Environ *e ){
	lhs=lhs->semant(e);
	rhs=rhs->semant(e);
	if( lhs->sem_type->structType() || rhs->sem_type->structType() ){
		ex( "Arithmetic operator cannot be applied to custom type objects" );
	}
	if( lhs->sem_type==Type::string_type || rhs->sem_type==Type::string_type ){
		//one side is a string - only + operator...
		if( op!='+' ) ex( "Operator cannot be applied to strings" );
		sem_type=Type::string_type;
	}else if( op=='^' || lhs->sem_type==Type::float_type || rhs->sem_type==Type::float_type ){
		//It's ^, or one side is a float
		sem_type=Type::float_type;
	}else{
		//must be 2 ints
		sem_type=Type::int_type;
	}
	lhs=lhs->castTo( sem_type,e );
	rhs=rhs->castTo( sem_type,e );
	ConstNode *lc=lhs->constNode(),*rc=rhs->constNode();
	if( rc && (op=='/' || op==MOD) ){
		if( (sem_type==Type::int_type && !rc->intValue()) || (sem_type==Type::float_type && !rc->floatValue()) ){
			ex( "Division by zero" );
		}
	}
	if( lc && rc ){
		ExprNode *expr;
		if( sem_type==Type::string_type ){
			expr=d_new StringConstNode( lc->stringValue()+rc->stringValue() );
		}else if( sem_type==Type::int_type ){
			switch( op ){
			case '+':expr=d_new IntConstNode( lc->intValue()+rc->intValue() );break;
			case '-':expr=d_new IntConstNode( lc->intValue()-rc->intValue() );break;
			case '*':expr=d_new IntConstNode( lc->intValue()*rc->intValue() );break;
			case '/':expr=d_new IntConstNode( lc->intValue()/rc->intValue() );break;
			case MOD:expr=d_new IntConstNode( lc->intValue()%rc->intValue() );break;
			}
		}else{
			switch( op ){
			case '+':expr=d_new FloatConstNode( lc->floatValue()+rc->floatValue() );break;
			case '-':expr=d_new FloatConstNode( lc->floatValue()-rc->floatValue() );break;
			case '*':expr=d_new FloatConstNode( lc->floatValue()*rc->floatValue() );break;
			case '/':expr=d_new FloatConstNode( lc->floatValue()/rc->floatValue() );break;
			case MOD:expr=d_new FloatConstNode( fmod( lc->floatValue(),rc->floatValue() ) );break;
			case '^':expr=d_new FloatConstNode( pow( lc->floatValue(),rc->floatValue() ) );break;
			}
		}
		delete this;
		return expr;
	}
	return this;
}

TNode *ArithExprNode::translate( Codegen *g ){
	TNode *l=lhs->translate( g );
	TNode *r=rhs->translate( g );
	if( sem_type==Type::string_type ){
		return call( "__bbStrConcat",l,r );
	}
	int n=0;
	if( sem_type==Type::int_type ){
		switch( op ){
		case '+':n=IR_ADD;break;case '-':n=IR_SUB;break;
		case '*':n=IR_MUL;break;case '/':n=IR_DIV;break;
		case MOD:return call( "__bbMod",l,r );
		}
	}else{
		switch( op ){
		case '+':n=IR_FADD;break;case '-':n=IR_FSUB;break;
		case '*':n=IR_FMUL;break;case '/':n=IR_FDIV;break;
		case MOD:return fcall( "__bbFMod",l,r );
		case '^':return fcall( "__bbFPow",l,r );
		}
	}
	return d_new TNode( n,l,r );
}

json ArithExprNode::toJSON( Environ *e ){
	json tree;tree["@class"]="ArithExprNode";
	tree["sem_type"]=sem_type->toJSON();
	switch( op ){
	case '+':tree["op"]="ADD";break;
	case '-':tree["op"]="SUB";break;
	case '*':tree["op"]="MUL";break;
	case '/':tree["op"]="DIV";break;
	case MOD:tree["op"]="MOD";break;
	case '^':tree["op"]="SQR";break;
	}
	tree["lhs"]=lhs->toJSON( e );
	tree["rhs"]=rhs->toJSON( e );
	return tree;
}
