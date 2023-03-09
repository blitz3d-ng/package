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

#ifdef USE_LLVM
void StructDeclNode::translate2( Codegen_LLVM *g ){
	llvm::Constant *template_type0=0;
	auto template_typesary=llvm::ArrayType::get( llvm::PointerType::get( g->bbType,0 ),sem_type->fields->size()-1 );

	vector<llvm::Constant*> template_types;
	vector<llvm::Constant*> template_defs;
	vector<llvm::Type*> fields;
	fields.push_back( g->bbObj ); // base
	for( int k=0;k<sem_type->fields->size();++k ){
		Decl *field=sem_type->fields->decls[k];
		Type *type=field->type;
		type->llvmType( g->context.get() );
		if( VectorType *v=type->vectorType() ){
			fields.push_back( v->ty );
		}else{
			fields.push_back( type->llvmType( g->context.get() ) );
		}

		llvm::GlobalVariable* gt=0;
		string t;
		if( type==Type::int_type ) t="_bbIntType";
		else if( type==Type::float_type ) t="_bbFltType";
		else if( type==Type::string_type ) t="_bbStrType";
		else if( StructType *s=type->structType() ) gt=(llvm::GlobalVariable*)s->objty;
		else if( VectorType *v=type->vectorType() ) gt=v->llvmDef( g );

		if( !gt ) {
			gt=(llvm::GlobalVariable*)g->module->getOrInsertGlobal( t,g->bbType );
		}

		if( k>0 ) {
			template_types.push_back( gt );
		} else {
			template_type0=gt;
		}
	}

	sem_type->llvmTypeDef( g );
	sem_type->llvmType( g->context.get() );

	auto ty2=sem_type->deftype;

	llvm::StructType* ty=(llvm::StructType*)sem_type->structtype;
	ty->setBody( fields );

	vector<llvm::Type*> templatefields;
	templatefields.push_back( g->bbObjType ); // base
	templatefields.push_back( template_typesary );
	ty2->setBody( templatefields );

	auto temp=(llvm::GlobalVariable*)sem_type->objty;

	vector<llvm::Constant*> objdata;
	objdata.push_back( llvm::ConstantPointerNull::get( llvm::PointerType::get( g->bbField,0 ) ) );   // fields
	objdata.push_back( llvm::ConstantPointerNull::get( llvm::PointerType::get( g->bbObj,0 ) ) );     // next
	objdata.push_back( llvm::ConstantPointerNull::get( llvm::PointerType::get( g->bbObj,0 ) ) );     // prev
	objdata.push_back( llvm::ConstantPointerNull::get( llvm::PointerType::get( g->bbObjType,0 ) ) ); // type
	objdata.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt(64, -1) ) );                  // ref_cnt
	auto emptyobj=llvm::ConstantStruct::get( g->bbObj,objdata );

	vector<llvm::Constant*> objtypedata;
	objtypedata.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 64,5 ) ) ); // type

	for( int k=0;k<2;++k ){ // used/free
		vector<llvm::Value*> indices;
		indices.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt(32, 0) ) );
		indices.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt(32, 0) ) );
		indices.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt(32, k+1) ) );
		auto objptr=llvm::ConstantExpr::getGetElementPtr( ty2,temp,indices );

		vector<llvm::Constant*> objdata;
		objdata.push_back( llvm::ConstantPointerNull::get( llvm::PointerType::get( g->bbField,0 ) ) );   // fields
		objdata.push_back( objptr );     // next
		objdata.push_back( objptr );     // prev
		objdata.push_back( llvm::ConstantPointerNull::get( llvm::PointerType::get( g->bbObjType,0 ) ) ); // type
		objdata.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt(64, -1) ) );                  // ref_cnt
		auto defaultobj=llvm::ConstantStruct::get( g->bbObj,objdata );

		objtypedata.push_back( defaultobj );
	}

	objtypedata.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 64,sem_type->fields->size() ) ) ); // fieldCnt
	objtypedata.push_back( template_type0 );     // fieldTypes
	auto def=llvm::ConstantStruct::get( g->bbObjType,objtypedata );

	auto fieldtypesa=llvm::ConstantArray::get( template_typesary,template_types );

	vector<llvm::Constant*> structfields;
	structfields.push_back( def );
	structfields.push_back( fieldtypesa );
	auto init=llvm::ConstantStruct::get( ty2,structfields );
	temp->setInitializer( init );
}
#endif

json StructDeclNode::toJSON( Environ *e ){
	json tree;tree["@class"]="StructDeclNode";
	tree["pos"]=pos;
	tree["file"]=file;
	tree["ident"]=ident;
	tree["fields"]=fields->toJSON( e );
	tree["sem_type"]=sem_type->toJSON();
	return tree;
}
