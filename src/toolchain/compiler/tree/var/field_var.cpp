#include "field_var.h"

///////////////
// Field var //
///////////////
void FieldVarNode::semant( Environ *e ){
	expr=expr->semant( e );
	StructType *s=expr->sem_type->structType();
	if( !s ) ex( "Variable must be a Type" );
	sem_field=s->fields->findDecl( ident );
	if( !sem_field ) ex( "Type field not found" );
	sem_type=sem_field->type;
}

TNode *FieldVarNode::translate( Codegen *g ){
	TNode *t=expr->translate( g );
	if( g->debug ) t=jumpf( t,"__bbNullObjEx" );
	t=mem( t );if( g->debug ) t=jumpf( t,"__bbNullObjEx" );
	return add( t,iconst( sem_field->offset ) );
}

json FieldVarNode::toJSON( Environ *e ){
	json tree;tree["@class"]="FieldVarNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["expr"]=expr->toJSON( e );
	tree["ident"]=ident;
	tree["tag"]=tag;
	tree["sem_field"]=sem_field->toJSON();
	return tree;
}
