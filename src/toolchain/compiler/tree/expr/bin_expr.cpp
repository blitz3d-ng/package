#include "bin_expr.h"
#include "int_const.h"

/////////////////////////////////////////////////////
// boolean expression - accepts ints, returns ints //
/////////////////////////////////////////////////////
ExprNode *BinExprNode::semant( Environ *e ){
	lhs=lhs->semant(e);lhs=lhs->castTo( Type::int_type,e );
	rhs=rhs->semant(e);rhs=rhs->castTo( Type::int_type,e );
	ConstNode *lc=lhs->constNode(),*rc=rhs->constNode();
	if( lc && rc ){
		ExprNode *expr;
		switch( op ){
		case AND:expr=d_new IntConstNode( lc->intValue() & rc->intValue() );break;
		case OR: expr=d_new IntConstNode( lc->intValue() | rc->intValue() );break;
		case XOR:expr=d_new IntConstNode( lc->intValue() ^ rc->intValue() );break;
		case SHL:expr=d_new IntConstNode( lc->intValue()<< rc->intValue() );break;
		case SHR:expr=d_new IntConstNode( (unsigned)lc->intValue()>>rc->intValue() );break;
		case SAR:expr=d_new IntConstNode( lc->intValue()>> rc->intValue() );break;
		}
		delete this;
		return expr;
	}
	sem_type=Type::int_type;
	return this;
}

TNode *BinExprNode::translate( Codegen *g ){
	TNode *l=lhs->translate( g );
	TNode *r=rhs->translate( g );
	int n=0;
	switch( op ){
	case AND:n=IR_AND;break;
	case OR:n=IR_OR;break;
	case XOR:n=IR_XOR;break;
	case SHL:n=IR_SHL;break;
	case SHR:n=IR_SHR;break;
	case SAR:n=IR_SAR;break;
	}
	return d_new TNode( n,l,r );
}

json BinExprNode::toJSON( Environ *e ){
	json tree;tree["@class"]="BinExprNode";
	tree["sem_type"]=sem_type->toJSON();
	switch( op ){
	case AND:tree["op"]="AND";break;
	case OR: tree["op"]="OR";break;
	case XOR:tree["op"]="XOR";break;
	case SHL:tree["op"]="SHL";break;
	case SHR:tree["op"]="SHR";break;
	case SAR:tree["op"]="SAR";break;
	}
	tree["lhs"]=lhs->toJSON( e );
	tree["rhs"]=rhs->toJSON( e );
	return tree;
}
