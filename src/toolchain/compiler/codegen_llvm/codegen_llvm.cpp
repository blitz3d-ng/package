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

Codegen_LLVM::Codegen_LLVM( bool debug ):debug(debug),breakBlock(0) {
	context=std::make_unique<llvm::LLVMContext>();
	module=std::make_unique<llvm::Module>( "",*context );

	builder=new IRBuilder<>( *context );
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
}

bool Codegen_LLVM::verify(){
	if( llvm::verifyModule( *module,&llvm::errs() ) ){
		llvm::errs()<<'\n';
		dumpToStderr();
		exit( 1 );
	}

	return false;
}

int Codegen_LLVM::dumpToObj( const std::string &path ) {
	optimize();

	InitializeAllTargetInfos();
	InitializeAllTargets();
	InitializeAllTargetMCs();
	InitializeAllAsmParsers();
	InitializeAllAsmPrinters();

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
	module->print(errs(), nullptr);
}
