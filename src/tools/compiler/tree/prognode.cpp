#include "prognode.h"
#include "../libs.h"

//////////////////
// The program! //
//////////////////
Environ *ProgNode::semant( Environ *e ){

	file_lab=genLabel();

	StmtSeqNode::reset( stmts->file,file_lab );

	a_ptr<Environ> env( d_new Environ( genLabel(),Type::int_type,0,e ) );

	consts->proto( env->decls,env );
	structs->proto( env->typeDecls,env );
	structs->semant( env );
	funcs->proto( env->funcDecls,env );
	stmts->semant( env );
	funcs->semant( env );
	datas->proto( env->decls,env );
	datas->semant( env );

	sem_env=env.release();
	return sem_env;
}

void ProgNode::translate( Codegen *g,const std::vector<UserFunc> &usrfuncs ){

	int k;

	if( g->debug ) g->s_data( stmts->file,file_lab );

	//enumerate locals
	int size=enumVars( sem_env );

	//'Main' label
	g->enter( "__MAIN",size );

	//reset data pointer
	g->code( call( "__bbRestore",global( "__DATA" ) ) );

	//load external libs
	g->code( call( "__bbLoadLibs",global( "__LIBS" ) ) );

	//call main program
	g->code( jsr( sem_env->funcLabel+"_begin" ) );
	g->code( jump( sem_env->funcLabel+"_leave" ) );
	g->label( sem_env->funcLabel+"_begin" );

	//create locals
	TNode *t=createVars( sem_env );
	if( t ) g->code( t );
	if( g->debug ){
		std::string t=genLabel();
		g->s_data( "<main program>",t );
		g->code( call( "__bbDebugEnter",local(0),iconst((bb_int_t)sem_env),global(t) ) );
	}

	//no user funcs used!
	usedfuncs.clear();

	//program statements
	stmts->translate( g );

	//emit return
	g->code( ret() );

	//check labels
	for( k=0;k<sem_env->labels.size();++k ){
		if( sem_env->labels[k]->def<0 )	ex( "Undefined label '"+sem_env->labels[k]->name+"'",sem_env->labels[k]->ref,stmts->file );
	}

	//leave main program
	g->label( sem_env->funcLabel+"_leave" );
	t=deleteVars( sem_env );
	if( g->debug ) t=d_new TNode( IR_SEQ,call( "__bbDebugLeave" ),t );
	g->leave( t,0 );

	//structs
	structs->translate( g );

	//non-main functions
	funcs->translate( g );

	//data
	datas->translate( g );

	//library functions
	std::map<std::string,std::vector<int> > libFuncs;

	//lib ptrs
	g->flush();
	g->align_data(4);
	for( k=0;k<usrfuncs.size();++k ){
		const UserFunc &fn=usrfuncs[k];

		if( !usedfuncs.count(fn.ident) ) continue;

		libFuncs[fn.lib].push_back( k );

		g->i_data( 0,"_f"+fn.ident );
	}

	//LIBS chunk
	g->flush();
	g->label( "__LIBS" );
	std::map<std::string,std::vector<int> >::const_iterator lf_it;
	for( lf_it=libFuncs.begin();lf_it!=libFuncs.end();++lf_it ){

		//lib name
		g->s_data( lf_it->first );

		const std::vector<int> &fns=lf_it->second;

		for( int j=0;j<fns.size();++j ){
			const UserFunc &fn=usrfuncs[ fns[j] ];

			//proc name
			g->s_data( fn.proc );

			g->p_data( "_f"+fn.ident );
		}
		g->s_data( "" );
	}
	g->s_data( "" );

	//DATA chunk
	g->flush();
	g->align_data( 4 );
	g->label( "__DATA" );
	datas->transdata( g );
	g->i_data(0);

	//Thats IT!
	g->flush();
}

json ProgNode::toJSON( Environ *e ){
	json tree;tree["@class"]="ProgNode";
	tree["debug"]=debug;
	tree["modules"]=modules;
	tree["funcs"]=funcs->toJSON( e );
	tree["structs"]=structs->toJSON( e );

	tree["types"]=json::array();
	for( int k=0;k<e->types.size();++k ){
		tree["types"].push_back( e->types[k]->toFullJSON() );
	}

	tree["arrays"]=json::array();
	tree["globals"]=json::array();
	tree["locals"]=json::array();
	for( int k=0;k<e->decls->size();++k ){
		Decl *d=e->decls->decls[k];
		switch( d->kind ){
		case DECL_LOCAL:
			tree["locals"].push_back( d->toJSON() );
			break;
		case DECL_GLOBAL:
			tree["globals"].push_back( d->toJSON() );
			break;
		case DECL_ARRAY:
			tree["arrays"].push_back( d->toJSON() );
			break;
		}
	}

	tree["data"]=datas->toJSON( e );

	tree["stmts"]=stmts->toJSON( e );
	return tree;
}

#ifdef USE_LLVM
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Verifier.h>

void ProgNode::translate2( Codegen_LLVM *g,const std::vector<UserFunc> &userfuncs ){
	std::string filepath=stmts->file; // TODO: should make this absolute
	size_t slash_idx=filepath.find_last_of("/\\");
	std::string filedir=filepath.substr( 0,slash_idx );
	std::string filename=filepath.substr( slash_idx+1 );

	g->module->setModuleIdentifier( filepath );
	g->module->setSourceFileName( filepath );

	if( g->debug ){
		g->dbgCU=g->dbgBuilder->createCompileUnit( llvm::dwarf::DW_LANG_C,g->dbgBuilder->createFile( filename,filedir ),"blitzcc",0,"",0 );
	}

	int k;
	for( k=0;k<sem_env->decls->size();++k ){
		Decl *d=sem_env->decls->decls[k];
		if( d->kind!=DECL_GLOBAL ) continue;

		auto ty=d->type->llvmType( g->context.get() );
		auto glob=new llvm::GlobalVariable( *g->module,ty,false,llvm::GlobalValue::ExternalLinkage,nullptr,d->name );

		llvm::Constant *init=0;
		if( d->type->intType() ){
			init=g->constantInt( 0 );
		}if( d->type->floatType() ){
			init=g->constantFloat( 0.0 );
		}if( d->type->stringType() || d->type->structType() || d->type->vectorType() ){
			init=llvm::ConstantPointerNull::get( (llvm::PointerType*)ty );
		}

		glob->setInitializer( init );
		d->ptr=glob;
	}

	llvm::Type *bool_ty=llvm::Type::getInt1Ty( *g->context );
	std::vector<llvm::Type*> none( 0,g->voidTy );

	auto mainFt=llvm::FunctionType::get( g->voidTy,none,false );
	g->bbMain=llvm::Function::Create( mainFt,llvm::Function::ExternalLinkage,"bbMain",g->module.get() );

	if( g->debug ){
		llvm::DIFile *Unit=g->dbgBuilder->createFile( filename,filedir );
		llvm::DIScope *FContext=Unit;
		unsigned LineNo=0,ScopeLine=0;
		llvm::SmallVector<llvm::Metadata*,8> EltTys;
		llvm::DISubroutineType *sub=g->dbgBuilder->createSubroutineType( g->dbgBuilder->getOrCreateTypeArray( EltTys ) );
		llvm::DISubprogram *SP = g->dbgBuilder->createFunction( FContext,"bbMain",llvm::StringRef(),Unit,LineNo,sub,ScopeLine,llvm::DINode::FlagPrototyped,llvm::DISubprogram::SPFlagDefinition );
		g->bbMain->setSubprogram( SP );

		g->dbgBlocks.push_back( SP );
	}

	auto block = llvm::BasicBlock::Create( *g->context,"entry",g->bbMain );
	g->builder->SetInsertPoint( block );

	datas->translate2( g );
	auto ty=llvm::ArrayType::get( g->intTy,g->data_values.size() );
	g->bbData=new llvm::GlobalVariable( *g->module,ty,false,llvm::GlobalValue::ExternalLinkage,nullptr,"bbData" );
	g->bbData->setInitializer( llvm::ConstantArray::get( ty,g->data_values ) );

	structs->translate2( g );

	funcs->translate2( g );

	g->builder->SetInsertPoint( block );

	g->restoreData( 0 ); // reset data pointer

	createVars2( sem_env,g );
	stmts->translate2( g );

	g->builder->CreateRetVoid();

	if( g->debug ){
		g->dbgBlocks.pop_back();
		g->dbgBuilder->finalize();
	}
	g->verify();
}
#endif

json ProgNode::toJSON( bool dbg ){
	debug = dbg;
	return toJSON( sem_env );
}
