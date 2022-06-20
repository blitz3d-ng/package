#include "codegen_llvm.h"
#include <llvm/ADT/Any.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
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
using namespace llvm;

#include <cstdarg>

Codegen_LLVM::Codegen_LLVM( bool debug ):debug(debug),breakBlock(0) {
	context=std::make_unique<llvm::LLVMContext>();
	module=std::make_unique<llvm::Module>( "",*context );

	builder=new IRBuilder<>( *context );

	// initialize stdlib constructs...
	bbType=llvm::StructType::create( *context,"BBType" );
	bbField=llvm::StructType::create( *context,"BBField" );
	bbObj=llvm::StructType::create( *context,"BBObj" );
	bbObjType=llvm::StructType::create( *context,"BBObjType" );
	bbArray=llvm::StructType::create( *context,"BBArray" );
	bbVecType=llvm::StructType::create( *context,"BBVecType" );

	std::vector<llvm::Type*> objels;
	objels.push_back( llvm::PointerType::get( bbField,0 ) );   // fields
	objels.push_back( llvm::PointerType::get( bbObj,0 ) );     // next
	objels.push_back( llvm::PointerType::get( bbObj,0 ) );     // prev
	objels.push_back( llvm::PointerType::get( bbObjType,0 ) ); // type
	objels.push_back( llvm::Type::getInt64Ty( *context ) );    // ref_cnt
	bbObj->setBody( objels );

	std::vector<llvm::Type*> objtypeels;
	objtypeels.push_back( llvm::Type::getInt64Ty( *context ) ); // type
	objtypeels.push_back( bbObj );                              // used
	objtypeels.push_back( bbObj );                              // free
	objtypeels.push_back( llvm::Type::getInt64Ty( *context ) ); // fieldCnt
	objtypeels.push_back( llvm::PointerType::get( bbType,0 ) ); // fieldTypes
	bbObjType->setBody( objtypeels );

	std::vector<llvm::Type*> arrayels;
	arrayels.push_back( llvm::PointerType::get( llvm::Type::getVoidTy( *context ),0 ) );  // data
	arrayels.push_back( llvm::PointerType::get( llvm::Type::getInt64Ty( *context ),0 ) ); // elementType
	arrayels.push_back( llvm::PointerType::get( llvm::Type::getInt64Ty( *context ),0 ) ); // dims
	bbArray->setBody( arrayels );

	std::vector<llvm::Type*> vecels;
	vecels.push_back( llvm::Type::getInt64Ty( *context ) ); // type
	vecels.push_back( llvm::Type::getInt64Ty( *context ) ); // size
	vecels.push_back( llvm::PointerType::get( bbType,0 ) ); // elementType
	bbVecType->setBody( vecels );
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

llvm::BasicBlock *Codegen_LLVM::getLabel( std::string &ident ){
	if( !labels[ident] ){
		labels[ident] = llvm::BasicBlock::Create( *context, "_l"+ident );
	}

	return labels[ident];
}

llvm::GlobalVariable *Codegen_LLVM::getArray( std::string &ident ){
	if( !arrays[ident] ){
		arrayTypes[ident]=llvm::StructType::create( *context,"_a"+ident+"data" );
		arrays[ident]=(llvm::GlobalVariable*)module->getOrInsertGlobal( "_a"+ident,arrayTypes[ident] );
	}

	return arrays[ident];
}

void Codegen_LLVM::optimize(){
	auto optimizer=std::make_unique<legacy::FunctionPassManager>( module.get() );
	optimizer->add( createInstructionCombiningPass() );
	optimizer->add( createReassociatePass() );
	optimizer->add( createNewGVNPass() );
	optimizer->add( createCFGSimplificationPass() );
	optimizer->add( createSCCPPass() );
	optimizer->add( createDeadCodeEliminationPass() );
	optimizer->doInitialization();

	for( auto &F:*module ){
		optimizer->run( F );
	}

	// module->print(errs(), nullptr);
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

int Codegen_LLVM::dumpToObj( bool compileonly,const std::string &path ) {
	if( compileonly ) {
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
		auto bbMain=builder->GetInsertBlock()->getParent();

		auto block = llvm::BasicBlock::Create( *context,"entry",main );
		builder->SetInsertPoint( block );
		builder->CreateRet( CallIntrinsic( "bbStart",int_ty,3,argc,argv,bbMain ) );
	}

	optimize();

	InitializeNativeTarget();
	InitializeNativeTargetAsmParser();
	InitializeNativeTargetAsmPrinter();

	auto TargetTriple = sys::getDefaultTargetTriple();
	module->setTargetTriple(TargetTriple);

	std::string Error;
	auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

	if (!Target) {
		errs() << Error;
		return 1;
	}

	auto CPU = "generic";
	auto Features = "";

	TargetOptions opt;
	auto RM = Optional<Reloc::Model>();
	auto TheTargetMachine = Target->createTargetMachine( TargetTriple,CPU,Features,opt,RM );

	module->setDataLayout( TheTargetMachine->createDataLayout() );

	std::error_code EC;
	raw_fd_ostream dest( path,EC,sys::fs::OF_None );

	if( EC ){
		errs()<<"Could not open file: "<<EC.message();
		return 1;
	}

	legacy::PassManager pass;
	auto FileType = CGFT_ObjectFile;

	if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
		errs() << "target can't emit a file of this type";
		return 1;
	}

	pass.run(*module);
	dest.flush();

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
