#include "var_decl.h"
#include "../expr/const.h"

////////////////////////////
// Simple var declaration //
////////////////////////////
void VarDeclNode::proto( DeclSeq *d,Environ *e ){

	Type *ty=tagType( tag,e );
	if( !ty ) ty=Type::int_type;
	ConstType *defType=0;

	if( expr ){
		expr=expr->semant( e );
		expr=expr->castTo( ty,e );
		if( constant || (kind&DECL_PARAM) ){
			ConstNode *c=expr->constNode();
			if( !c ) ex( "Expression must be constant" );
			if( ty==Type::int_type ) ty=d_new ConstType( c->intValue() );
			else if( ty==Type::float_type ) ty=d_new ConstType( c->floatValue() );
			else ty=d_new ConstType( c->stringValue() );
			e->types.push_back( ty );
			delete expr;expr=0;
		}
		if( kind&DECL_PARAM ){
			defType=ty->constType();ty=defType->valueType;
		}
	}else if( constant ) ex( "Constants must be initialized" );

	Decl *decl=d->insertDecl( ident,ty,kind,defType );
	if( !decl ) ex( "Duplicate variable name" );
	if( expr ) sem_var=d_new DeclVarNode( decl );
}

void VarDeclNode::semant( Environ *e ){
}

void VarDeclNode::translate( Codegen *g ){
	if( kind & DECL_GLOBAL ){
		g->align_data( 4 );
		g->i_data( 0,"_v"+ident );
	}
	if( expr ) g->code( sem_var->store( g,expr->translate( g ) ) );
}

json VarDeclNode::toJSON( Environ *e ){
	json tree;tree["@class"]="VarDeclNode";
	tree["pos"]=pos;
	tree["file"]=file;
	tree["ident"]=ident;
	tree["tag"]=tag;
	tree["kind"]=kind;
	tree["constant"]=constant;
	if( expr ) tree["expr"]=expr->toJSON( e );
	if( sem_var ) tree["sem_var"]=sem_var->toJSON( e );
	return tree;
}
