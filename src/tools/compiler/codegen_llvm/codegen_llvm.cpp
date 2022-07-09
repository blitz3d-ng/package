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
using namespace llvm;

#include <cstdarg>

Codegen_LLVM::Codegen_LLVM( bool debug ):debug(debug),breakBlock(0) {
	InitializeNativeTarget();
	InitializeNativeTargetAsmPrinter();
	InitializeNativeTargetAsmParser();

	context=std::make_unique<llvm::LLVMContext>();
	module=std::make_unique<llvm::Module>( "",*context );

	builder=new IRBuilder<>( *context );

	context->enableOpaquePointers();

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

	// -------
	auto triple=sys::getDefaultTargetTriple();

	std::string err;
	auto target = TargetRegistry::lookupTarget( triple,err );
	if( !target ){
		errs()<<err;
		return;
	}

	auto cpu="generic",features="";
	TargetOptions opt;
	auto rm=Optional<Reloc::Model>();
	targetMachine=target->createTargetMachine( triple,cpu,features,opt,rm );

	module->setTargetTriple( triple );
	module->setDataLayout( targetMachine->createDataLayout() );

	if( debug ){
		targetMachine->setOptLevel( CodeGenOpt::Level::None );
		targetMachine->setFastISel( false );
		targetMachine->setGlobalISel( false );
	}
}

Value *Codegen_LLVM::CallIntrinsic( const std::string &symbol,llvm::Type *typ,int n,... ){
	Function *func=module->getFunction( symbol );

	std::vector<Value *> args;
	va_list vl;
	va_start( vl,n );
	for( int k=0;k<n;k++ ){
		args.push_back( va_arg( vl,Value* ) );
	}
	va_end(vl);

	if( !func ){
		std::vector<Type*> decls( args.size() );

		for( int k=0;k<args.size();k++ ){
			decls[k]=args[k]->getType();
		}

		auto ft = FunctionType::get( typ,decls,false );
		func=Function::Create( ft,GlobalValue::ExternalLinkage,symbol,module.get() );
		func->setCallingConv( CallingConv::C );
	}

	return builder->CreateCall(func, args);
}

Value *Codegen_LLVM::CastToObjPtr( llvm::Value *v ){
	auto objty=llvm::PointerType::get( bbObj,0 );
	return builder->CreateBitOrPointerCast( v,objty );
}

Value *Codegen_LLVM::CastToArrayPtr( llvm::Value *v ){
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
	LoopAnalysisManager LAM;
	FunctionAnalysisManager FAM;
	CGSCCAnalysisManager CGAM;
	ModuleAnalysisManager MAM;

	PassBuilder PB( targetMachine );

	PB.registerModuleAnalyses( MAM );
	PB.registerCGSCCAnalyses( CGAM );
	PB.registerFunctionAnalyses( FAM );
	PB.registerLoopAnalyses( LAM );
	PB.crossRegisterProxies( LAM,FAM,CGAM,MAM );

	ModulePassManager MPM = PB.buildPerModuleDefaultPipeline( OptimizationLevel::O3 );

	MPM.run( *module,MAM );
}

bool Codegen_LLVM::verify(){
	std::string err;
	raw_string_ostream os( err );

	if( llvm::verifyModule( *module,&os ) ){
		dumpToStderr();
#ifdef BB_POSIX
		errs()<<"\033[21;31m";
#endif
		errs()<<err;
#ifdef BB_POSIX
		errs()<<"\033[0m\n";
#endif

		exit( 1 );
	}

	return false;
}

void Codegen_LLVM::injectMain(){
	llvm::Type *int_ty=llvm::Type::getInt32Ty( *context );
	llvm::Type *charpp_ty=llvm::PointerType::get( llvm::PointerType::get( llvm::Type::getInt8Ty( *context ),0 ),0 );
	llvm::Type *void_type=llvm::Type::getVoidTy( *context );

	std::vector<llvm::Type*> args;
	args.push_back( int_ty );
	args.push_back( charpp_ty );
	auto ft=llvm::FunctionType::get( int_ty,args,false );
	auto main=llvm::Function::Create( ft,llvm::Function::ExternalLinkage,"main",module.get() );

	auto argc=main->getArg( 0 );
	auto argv=main->getArg( 1 );

	auto block = llvm::BasicBlock::Create( *context,"entry",main );
	builder->SetInsertPoint( block );
	builder->CreateRet( CallIntrinsic( "bbStart",int_ty,3,argc,argv,bbMain ) );
}

int Codegen_LLVM::dumpToObj( std::string &out ) {
	raw_string_ostream sstr( out );
	buffer_ostream dest( sstr );

	legacy::PassManager pass;
	if( targetMachine->addPassesToEmitFile( pass,(raw_pwrite_stream &)sstr,0,CGFT_ObjectFile ) ){
		errs() << "target can't emit a file of this type";
		return 1;
	}

	pass.run( *module );

	sstr.flush();

	return 0;
}

void Codegen_LLVM::dumpToStderr() {
#ifdef BB_POSIX
	errs()<<"\033[21;90m";
#endif
	module->print( errs(),nullptr );
#ifdef BB_POSIX
	errs()<<"\033[0m\n";
#endif
}
