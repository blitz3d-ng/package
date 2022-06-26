#include "jit_orc.h"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>

#ifdef BB_WINDOWS
#define BB_SO_EXT "dll"
#endif
#ifdef BB_MACOS
#define BB_SO_EXT "dylib"
#endif
#ifdef BB_LINUX
#define BB_SO_EXT "so"
#endif

JIT_ORC::JIT_ORC( std::unique_ptr<llvm::orc::ExecutionSession> ES,std::unique_ptr<llvm::orc::EPCIndirectionUtils> EPCIU,llvm::orc::JITTargetMachineBuilder JTMB, llvm::DataLayout DL ):
	ES(std::move(ES)), EPCIU(std::move(EPCIU)), DL(std::move(DL)),
	Mangle(*this->ES, this->DL),
	ObjectLayer(*this->ES,[]() { return std::make_unique<llvm::SectionMemoryManager>(); }),
	CompileLayer(*this->ES, ObjectLayer, std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(JTMB))),
	OptimizeLayer(*this->ES, CompileLayer, optimizeModule),
	MainJD(this->ES->createBareJITDylib( "<main>" )
){
}

JIT_ORC::~JIT_ORC() {
	if( auto Err=ES->endSession() ) ES->reportError( std::move(Err) );
	if( auto Err=EPCIU->cleanup() ) ES->reportError( std::move(Err) );
}

void JIT_ORC::handleLazyCallThroughError() {
	llvm::errs() << "LazyCallThrough error: Could not find function body";
	exit(1);
}

llvm::Expected<std::unique_ptr<JIT_ORC>> JIT_ORC::Create() {
  auto EPC = llvm::orc::SelfExecutorProcessControl::Create();
  if (!EPC)
    return EPC.takeError();

  auto ES = std::make_unique<llvm::orc::ExecutionSession>(std::move(*EPC));

  auto EPCIU = llvm::orc::EPCIndirectionUtils::Create(ES->getExecutorProcessControl());
  if (!EPCIU)
    return EPCIU.takeError();

  (*EPCIU)->createLazyCallThroughManager(
      *ES, llvm::pointerToJITTargetAddress(&handleLazyCallThroughError));

  if (auto Err = setUpInProcessLCTMReentryViaEPCIU(**EPCIU))
    return std::move(Err);

  llvm::orc::JITTargetMachineBuilder JTMB(
      ES->getExecutorProcessControl().getTargetTriple());

  auto DL = JTMB.getDefaultDataLayoutForTarget();
  if (!DL)
    return DL.takeError();

  return std::make_unique<JIT_ORC>(std::move(ES), std::move(*EPCIU),
                                           std::move(JTMB), std::move(*DL));
}

const llvm::DataLayout &JIT_ORC::getDataLayout() const { return DL; }

llvm::orc::JITDylib &JIT_ORC::getMainJITDylib() { return MainJD; }

llvm::Error JIT_ORC::addModule(llvm::orc::ThreadSafeModule TSM, llvm::orc::ResourceTrackerSP RT) {
	if (!RT) RT = MainJD.getDefaultResourceTracker();
	return OptimizeLayer.add(RT, std::move(TSM));
}

llvm::Expected<llvm::JITEvaluatedSymbol> JIT_ORC::lookup(llvm::StringRef Name) {
	return ES->lookup({&MainJD}, Mangle(Name.str()));
}

int JIT_ORC::run( Runtime *runtime, Codegen_LLVM *codegen, const std::string &home, const std::string &rt ) {
	MainJD.addGenerator(cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(DL.getGlobalPrefix())));
	MainJD.addGenerator(cantFail(llvm::orc::DynamicLibrarySearchGenerator::Load( runtime->path.c_str(),DL.getGlobalPrefix() )));

	auto RT = getMainJITDylib().createResourceTracker();

	auto TSM = llvm::orc::ThreadSafeModule( std::move(codegen->module),std::move(codegen->context) );
	cantFail(addModule(std::move(TSM), RT));

	auto main_sym  = cantFail(lookup("bbMain"));
	auto start_sym = cantFail(lookup("bbStart"));
	BBMAIN  bbMain  = (BBMAIN)main_sym.getAddress();
	BBSTART bbStart = (BBSTART)start_sym.getAddress();

	char *argv[1] = { "blitzcc" };
	int retcode = bbStart( 1, argv, bbMain );
	cantFail(RT->remove());
	return retcode;
}

llvm::Expected<llvm::orc::ThreadSafeModule> JIT_ORC::optimizeModule(llvm::orc::ThreadSafeModule TSM, const llvm::orc::MaterializationResponsibility &R) {
	// TSM.withModuleDo([](llvm::Module &M) {
	// 	auto FPM = std::make_unique<llvm::legacy::FunctionPassManager>(&M);
	//
	// 	FPM->add( llvm::createInstructionCombiningPass() );
	// 	FPM->add( llvm::createReassociatePass() );
	// 	FPM->add( llvm::createGVNPass() );
	// 	FPM->add( llvm::createCFGSimplificationPass() );
	// 	FPM->doInitialization();
	//
	// 	for( auto &F:M ){
	// 		FPM->run(F);
	// 	}
	// });

	return std::move(TSM);
}
