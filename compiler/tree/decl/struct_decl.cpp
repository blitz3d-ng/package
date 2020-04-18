#include "struct_decl.h"

//////////////////////
// Type Declaration //
//////////////////////
void StructDeclNode::proto( DeclSeq *d,Environ *e ){
	sem_type=d_new StructType( ident,d_new DeclSeq() );
	if( !d->insertDecl( ident,sem_type,DECL_STRUCT ) ){
		delete sem_type;ex( "Duplicate identifier" );
	}
	e->types.push_back( sem_type );
}

void StructDeclNode::semant( Environ *e ){
	fields->proto( sem_type->fields,e );
	for( int k=0;k<sem_type->fields->size();++k ) sem_type->fields->decls[k]->offset=k*4;
}

void StructDeclNode::translate( Codegen *g ){

	//translate fields
	fields->translate( g );

	//type ID
	g->align_data( 4 );
	g->i_data( 5,"_t"+ident );

	//used and free lists for type
	int k;
	for( k=0;k<2;++k ){
		string lab=genLabel();
		g->i_data( 0,lab );	//fields
		g->p_data( lab );	//next
		g->p_data( lab );	//prev
		g->i_data( 0 );		//type
		g->i_data( -1 );	//ref_cnt
	}

	//number of fields
	g->i_data( sem_type->fields->size() );

	//type of each field
	for( k=0;k<sem_type->fields->size();++k ){
		Decl *field=sem_type->fields->decls[k];
		Type *type=field->type;
		string t;
		if( type==Type::int_type ) t="__bbIntType";
		else if( type==Type::float_type ) t="__bbFltType";
		else if( type==Type::string_type ) t="__bbStrType";
		else if( StructType *s=type->structType() ) t="_t"+s->ident;
		else if( VectorType *v=type->vectorType() ) t=v->label;
		g->p_data( t );
	}

}

json StructDeclNode::toJSON( Environ *e ){
	json tree;tree["@class"]="StructDeclNode";
	tree["pos"]=pos;
	tree["file"]=file;
	tree["ident"]=ident;
	tree["fields"]=fields->toJSON( e );
	tree["sem_type"]=sem_type->toJSON();
	return tree;
}
