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

#ifdef USE_LLVM
llvm::Value *FieldVarNode::translate2( Codegen_LLVM *g ){
	std::vector<llvm::Value*> indices;
	indices.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 32,0 ) ) );
	indices.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 32,sem_field->offset/4+1 ) ) );
	auto el=g->builder->CreateGEP( expr->sem_type->structType()->structtype,expr->translate2( g ),indices );

	return el;
}
#endif

json FieldVarNode::toJSON( Environ *e ){
	json tree;tree["@class"]="FieldVarNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["expr"]=expr->toJSON( e );
	tree["ident"]=ident;
	tree["tag"]=tag;
	tree["sem_field"]=sem_field->toJSON();
	return tree;
}
