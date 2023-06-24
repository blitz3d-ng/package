#include "codegen_llvm.h"
#include <llvm/ADT/Any.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Object/Binary.h>

#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/IPO.h>

#include <cstdarg>
#include <iostream>

Codegen_LLVM::Codegen_LLVM( bool debug ):debug(debug),breakBlock(0) {
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();

	context=std::make_unique<llvm::LLVMContext>();
	module=std::make_unique<llvm::Module>( "",*context );

	builder=new llvm::IRBuilder<>( *context );

	context->setOpaquePointers(true);

	voidTy=llvm::Type::getVoidTy( *context );
	intTy=llvm::Type::getInt64Ty( *context );
	voidPtr=llvm::PointerType::get( *context,0 );

	// initialize stdlib constructs...
	bbType=llvm::StructType::create( *context,"BBType" );
	bbField=llvm::StructType::create( *context,"BBField" );
	bbObj=llvm::StructType::create( *context,"BBObj" );
	bbObjType=llvm::StructType::create( *context,"BBObjType" );
	bbArray=llvm::StructType::create( *context,"BBArray" );
	bbVecType=llvm::StructType::create( *context,"BBVecType" );

	bbObjPtr=llvm::PointerType::get( bbObj,0 );

	std::vector<llvm::Type*> objels;
	objels.push_back( llvm::PointerType::get( bbField,0 ) );   // fields
	objels.push_back( bbObjPtr );     // next
	objels.push_back( bbObjPtr );     // prev
	objels.push_back( llvm::PointerType::get( bbObjType,0 ) ); // type
	objels.push_back( intTy );    // ref_cnt
	bbObj->setBody( objels );

	std::vector<llvm::Type*> objtypeels;
	objtypeels.push_back( intTy ); // type
	objtypeels.push_back( bbObj );                              // used
	objtypeels.push_back( bbObj );                              // free
	objtypeels.push_back( intTy ); // fieldCnt
	objtypeels.push_back( llvm::PointerType::get( bbType,0 ) ); // fieldTypes
	bbObjType->setBody( objtypeels );

	std::vector<llvm::Type*> arrayels;
	arrayels.push_back( voidPtr );  // data
	arrayels.push_back( intTy ); // elementType
	arrayels.push_back( intTy ); // dims
	bbArray->setBody( arrayels );

	std::vector<llvm::Type*> vecels;
	vecels.push_back( intTy ); // type
	vecels.push_back( intTy ); // size
	vecels.push_back( llvm::PointerType::get( bbType,0 ) ); // elementType
	bbVecType->setBody( vecels );

	if( debug ){
		dbgBuilder=new llvm::DIBuilder( *module );
		dbgFloatTy=dbgBuilder->createBasicType( "double",64,llvm::dwarf::DW_ATE_float );
	}
}

void Codegen_LLVM::SetTarget( const ::Target &t ){
	target=t.type;

	if( !t.host ){
		llvm::InitializeAllTargetInfos();
		llvm::InitializeAllTargets();
		llvm::InitializeAllTargetMCs();
		llvm::InitializeAllAsmParsers();
		llvm::InitializeAllAsmPrinters();
	}

	// TODO: remove this bad hack
	auto tt=t.triple;
	if( tt.rfind("armeabi-v7a", 0) == 0) {
		tt="arm";
	}else if( tt.rfind("x86-", 0) == 0) {
		tt="i686";
	}

	std::string err;
	auto targ = llvm::TargetRegistry::lookupTarget( tt,err );
	if( !targ ){
		llvm::errs()<<err<<'\n';
		for( auto tt:llvm::TargetRegistry::targets() ){
			std::cout<<"  "<<tt.getName()<<" - "<<tt.getShortDescription()<<std::endl;
		}
		exit( 1 );
	}

	auto cpu="generic",features="";
	llvm::TargetOptions opt;
	auto rm=std::optional<llvm::Reloc::Model>( llvm::Reloc::PIC_ );
	auto cm=std::optional<llvm::CodeModel::Model>( llvm::CodeModel::Model::Small );
	targetMachine=targ->createTargetMachine( t.triple,cpu,features,opt,rm,cm );

	module->setTargetTriple( t.triple );
	module->setDataLayout( targetMachine->createDataLayout() );
}

llvm::Value *Codegen_LLVM::CallIntrinsic( const std::string &symbol,llvm::Type *typ,int n,... ){
	llvm::Function *func=module->getFunction( symbol );

	std::vector<llvm::Value *> args;
	va_list vl;
	va_start( vl,n );
	for( int k=0;k<n;k++ ){
		args.push_back( va_arg( vl,llvm::Value* ) );
	}
	va_end(vl);

	if( !func ){
		std::vector<llvm::Type*> decls( args.size() );

		for( int k=0;k<args.size();k++ ){
			decls[k]=args[k]->getType();
		}

		auto ft = llvm::FunctionType::get( typ,decls,false );
		func=llvm::Function::Create( ft,llvm::GlobalValue::ExternalLinkage,symbol,module.get() );
		func->setCallingConv( llvm::CallingConv::C );
	}

	return builder->CreateCall(func, args);
}

llvm::Value *Codegen_LLVM::CastToObjPtr( llvm::Value *v ){
	auto objty=llvm::PointerType::get( bbObj,0 );
	return builder->CreateBitOrPointerCast( v,objty );
}

llvm::Value *Codegen_LLVM::CastToArrayPtr( llvm::Value *v ){
	auto aryty=llvm::PointerType::get( bbArray,0 );
	return builder->CreateBitOrPointerCast( v,aryty );
}

llvm::Constant *Codegen_LLVM::constantInt( int i ){
	return llvm::ConstantInt::get( *context,llvm::APInt( 64,i ) );
}

llvm::Constant *Codegen_LLVM::constantFloat( double f ){
	return llvm::ConstantFP::get( *context,llvm::APFloat( f ) );
}

llvm::Constant *Codegen_LLVM::constantString( const std::string &s ){
	auto v=strings[s];
	if( !v ){
		v=strings[s]=builder->CreateGlobalStringPtr( s );
	}
	return v;
}

llvm::BasicBlock *Codegen_LLVM::getLabel( std::string &ident ){
	if( !labels[ident] ){
		labels[ident] = llvm::BasicBlock::Create( *context, "_l"+ident );
	}

	return labels[ident];
}

llvm::GlobalVariable *Codegen_LLVM::getArray( std::string &ident, int dims ){
	if( !arrays[ident] ){
		std::vector<llvm::Type*> els;
		els.push_back( bbArray );  // base
		for( int k=0;k<dims;++k ) {
			els.push_back( llvm::Type::getInt64Ty( *context ) ); // scales
		}
		arrayTypes[ident]=llvm::StructType::create( *context,els,"_a"+ident+"data" );
		arrays[ident]=(llvm::GlobalVariable*)module->getOrInsertGlobal( "_a"+ident,arrayTypes[ident] );
	}

	return arrays[ident];
}

void Codegen_LLVM::optimize(){
	if( debug ){
		return;
	}

	// until the gosub implementation is improved, we have to relax optimization
	// to avoid long build times...
	if( gosubUsed ){
		targetMachine->setOptLevel( llvm::CodeGenOpt::Level::None );
		targetMachine->setFastISel( false );
		targetMachine->setGlobalISel( false );
		return;
	}

	llvm::LoopAnalysisManager LAM;
	llvm::FunctionAnalysisManager FAM;
	llvm::CGSCCAnalysisManager CGAM;
	llvm::ModuleAnalysisManager MAM;

	llvm::PassBuilder PB( targetMachine );

	PB.registerModuleAnalyses( MAM );
	PB.registerCGSCCAnalyses( CGAM );
	PB.registerFunctionAnalyses( FAM );
	PB.registerLoopAnalyses( LAM );
	PB.crossRegisterProxies( LAM,FAM,CGAM,MAM );

	llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline( llvm::OptimizationLevel::O3 );

	MPM.run( *module,MAM );
}

bool Codegen_LLVM::verify(){
	std::string err;
	llvm::raw_string_ostream os( err );

	if( llvm::verifyModule( *module,&os ) ){
		dumpToStderr();
#ifdef BB_POSIX
		llvm::errs()<<"\033[21;31m";
#endif
		llvm::errs()<<err;
#ifdef BB_POSIX
		llvm::errs()<<"\033[0m\n";
#endif

		exit( 1 );
	}

	return false;
}

void Codegen_LLVM::injectMain(){
	std::string mainsym="main";
	// TODO: not the cleanest, but works for now...
	if( target=="ios"||target=="ios-sim"||target=="android" ){
		mainsym="SDL_main";
	}else if( target=="ovr" ){
		mainsym="vr_main";
	}

	llvm::Type *int_ty=llvm::Type::getInt32Ty( *context );
	llvm::Type *charpp_ty=llvm::PointerType::get( llvm::PointerType::get( llvm::Type::getInt8Ty( *context ),0 ),0 );
	llvm::Type *void_type=llvm::Type::getVoidTy( *context );

	std::vector<llvm::Type*> args;
	args.push_back( int_ty );
	args.push_back( charpp_ty );
	auto ft=llvm::FunctionType::get( int_ty,args,false );
	auto main=llvm::Function::Create( ft,llvm::Function::ExternalLinkage,mainsym,module.get() );

	auto argc=main->getArg( 0 );
	auto argv=main->getArg( 1 );

	auto block = llvm::BasicBlock::Create( *context,"entry",main );
	builder->SetInsertPoint( block );
	builder->CreateRet( CallIntrinsic( "bbStart",int_ty,3,argc,argv,bbMain ) );
}

int Codegen_LLVM::dumpToObj( std::string &out ) {
	llvm::raw_string_ostream sstr( out );
	llvm::buffer_ostream dest( sstr );

	llvm::legacy::PassManager pass;
	if( targetMachine->addPassesToEmitFile( pass,dest,0,llvm::CGFT_ObjectFile ) ){
		llvm::errs()<<"target can't emit a file of this type\n";
		return 1;
	}

	pass.run( *module );

	sstr.flush();

	return 0;
}

void Codegen_LLVM::dumpToStderr() {
#ifdef BB_POSIX
	llvm::errs()<<"\033[21;90m";
#endif
	module->print( llvm::errs(),nullptr );
#ifdef BB_POSIX
	llvm::errs()<<"\033[0m\n";
#endif
}
