#include "array_var.h"

/////////////////
// Indexed Var //
/////////////////
void ArrayVarNode::semant( Environ *e ){
	exprs->semant( e );
	exprs->castTo( Type::int_type,e );
	Type *t=e->findType( tag );
	sem_decl=e->findDecl( ident );
	if( !sem_decl || !(sem_decl->kind&DECL_ARRAY) ) ex( "Array not found" );
	ArrayType *a=sem_decl->type->arrayType();
	if( t && t!=a->elementType ) ex( "array type mismtach" );
	if( a->dims!=exprs->size() ) ex( "incorrect number of dimensions" );
	sem_type=a->elementType;
}

TNode *ArrayVarNode::translate( Codegen *g ){
	TNode *t=0;
	for( int k=0;k<exprs->size();++k ){
		TNode *e=exprs->exprs[k]->translate( g );
		if( k ){
			TNode *s=mem( add( global( "_a"+ident ),iconst( k*4+8 ) ) );
			e=add( t,mul( e,s ) );
		}
		if( g->debug ){
			TNode *s=mem( add( global( "_a"+ident ),iconst( k*4+12 ) ) );
			t=jumpge( e,s,"__bbArrayBoundsEx" );
		}else t=e;
	}
	t=add( mem( global( "_a"+ident ) ),mul( t,iconst( 4 ) ) );
	return t;
}

json ArrayVarNode::toJSON( Environ *e ){
	json tree;tree["@class"]="ArrayVarNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["ident"]=ident;
	tree["tag"]=tag;
	tree["exprs"]=exprs->toJSON( e );
	tree["sem_decl"]=sem_decl->toJSON();
	return tree;
}
