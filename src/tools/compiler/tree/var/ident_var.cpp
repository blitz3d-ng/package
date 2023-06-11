#include "ident_var.h"

///////////////
// Ident var //
///////////////
void IdentVarNode::semant( Environ *e ){
	if( sem_decl ) return;
	Type *t=tagType( tag,e );if( !t ) t=Type::int_type;
	if( (sem_decl=e->findDecl( ident )) ){
		if( !(sem_decl->kind&(DECL_GLOBAL|DECL_LOCAL|DECL_PARAM)) ){
			ex( "Identifier '"+sem_decl->name+"' may not be used like this" );
		}
		Type *ty=sem_decl->type;
		if( ty->constType() ) ty=ty->constType()->valueType;
		if( tag.size() && t!=ty ) ex( "Variable type mismatch" );
	}else{
		if( e->strict ) ex( "Undeclared variable '"+ident+"'" );
		//ugly auto decl!
		sem_decl=e->decls->insertDecl( ident,t,DECL_LOCAL );
	}
	sem_type=sem_decl->type;
}

json IdentVarNode::toJSON( Environ *e ){
	json tree;tree["@class"]="IdentVarNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["ident"]=ident;
	tree["tag"]=tag;
	return tree;
}
