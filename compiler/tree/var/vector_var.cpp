#include "vector_var.h"
#include "../expr/const.h"

////////////////
// Vector var //
////////////////
void VectorVarNode::semant( Environ *e ){
	expr=expr->semant( e );
	vec_type=expr->sem_type->vectorType();
	if( !vec_type ) ex( "Variable must be a Blitz array" );
	if( vec_type->sizes.size()!=exprs->size() ) ex( "Incorrect number of subscripts" );
	exprs->semant( e );
	exprs->castTo( Type::int_type,e );
	for( int k=0;k<exprs->size();++k ){
		if( ConstNode *t=exprs->exprs[k]->constNode() ){
			if( t->intValue()>=vec_type->sizes[k] ){
				ex( "Blitz array subscript out of range" );
			}
		}
	}
	sem_type=vec_type->elementType;
}

TNode *VectorVarNode::translate( Codegen *g ){
	int sz=4;
	TNode *t=0;
	for( int k=0;k<exprs->size();++k ){
		TNode *p;
		ExprNode *e=exprs->exprs[k];
		if( ConstNode *t=e->constNode() ){
			p=iconst( t->intValue() * sz );
		}else{
			p=e->translate( g );
			if( g->debug ){
				p=jumpge( p,iconst( vec_type->sizes[k] ),"__bbVecBoundsEx" );
			}
			p=mul( p,iconst( sz ) );
		}
		sz=sz*vec_type->sizes[k];
		t=t ? add( t,p ) : p;
	}
	return add( t,expr->translate( g ) );
}

json VectorVarNode::toJSON( Environ *e ){
	json tree;tree["@class"]="VectorVarNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["expr"]=expr->toJSON( e );
	tree["exprs"]=exprs->toJSON( e );
	tree["vec_type"]=vec_type->toJSON();
	return tree;
}
