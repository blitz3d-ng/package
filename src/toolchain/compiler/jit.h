#ifndef JIT_H
#define JIT_H

#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/CompileOnDemandLayer.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/EPCIndirectionUtils.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/ExecutorProcessControl.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/IRTransformLayer.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>

#include "codegen_llvm/codegen_llvm.h"

typedef void (*BBMAIN)();
typedef int (*BBSTART)( int,char**,BBMAIN );

class BlitzJIT {
private:
  std::unique_ptr<llvm::orc::ExecutionSession> ES;
  std::unique_ptr<llvm::orc::EPCIndirectionUtils> EPCIU;

  llvm::DataLayout DL;

  llvm::orc::MangleAndInterner Mangle;
  llvm::orc::RTDyldObjectLinkingLayer ObjectLayer;
  llvm::orc::IRCompileLayer CompileLayer;
  llvm::orc::IRTransformLayer OptimizeLayer;

  llvm::orc::JITDylib &MainJD;
public:
  BlitzJIT(std::unique_ptr<llvm::orc::ExecutionSession> ES,
                  std::unique_ptr<llvm::orc::EPCIndirectionUtils> EPCIU,
                  llvm::orc::JITTargetMachineBuilder JTMB, llvm::DataLayout DL)
      : ES(std::move(ES)), EPCIU(std::move(EPCIU)), DL(std::move(DL)),
        Mangle(*this->ES, this->DL),
        ObjectLayer(*this->ES,
                    []() { return std::make_unique<llvm::SectionMemoryManager>(); }),
        CompileLayer(*this->ES, ObjectLayer,
                     std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(JTMB))),
        OptimizeLayer(*this->ES, CompileLayer, optimizeModule),
        MainJD(this->ES->createBareJITDylib("<main>")
	) {
  }

  ~BlitzJIT() {
    if (auto Err = ES->endSession())
      ES->reportError(std::move(Err));
    if (auto Err = EPCIU->cleanup())
      ES->reportError(std::move(Err));
  }

	static void handleLazyCallThroughError() {
		llvm::errs() << "LazyCallThrough error: Could not find function body";
		exit(1);
	}

  static llvm::Expected<std::unique_ptr<BlitzJIT>> Create() {
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

    return std::make_unique<BlitzJIT>(std::move(ES), std::move(*EPCIU),
                                             std::move(JTMB), std::move(*DL));
  }

  const llvm::DataLayout &getDataLayout() const { return DL; }

  llvm::orc::JITDylib &getMainJITDylib() { return MainJD; }

  llvm::Error addModule(llvm::orc::ThreadSafeModule TSM, llvm::orc::ResourceTrackerSP RT = nullptr) {
    if (!RT)
      RT = MainJD.getDefaultResourceTracker();

    return OptimizeLayer.add(RT, std::move(TSM));
  }

  llvm::Expected<llvm::JITEvaluatedSymbol> lookup(llvm::StringRef Name) {
    return ES->lookup({&MainJD}, Mangle(Name.str()));
  }

	int run( Codegen_LLVM *codegen, const std::string &home, const std::string &rt ) {
		std::string toolchain=home+"/toolchains/" BB_PLATFORM;
		string runtimeLib( toolchain+"/lib/" BB_ENV "/libruntime."+rt+".shared.dylib" );

		MainJD.addGenerator(cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(DL.getGlobalPrefix())));
		MainJD.addGenerator(cantFail(llvm::orc::DynamicLibrarySearchGenerator::Load( runtimeLib.c_str(),DL.getGlobalPrefix() )));

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

private:
  static llvm::Expected<llvm::orc::ThreadSafeModule>
  optimizeModule(llvm::orc::ThreadSafeModule TSM, const llvm::orc::MaterializationResponsibility &R) {
    TSM.withModuleDo([](llvm::Module &M) {
      auto FPM = std::make_unique<llvm::legacy::FunctionPassManager>(&M);

      FPM->add(llvm::createInstructionCombiningPass());
      FPM->add(llvm::createReassociatePass());
      FPM->add(llvm::createGVNPass());
      FPM->add(llvm::createCFGSimplificationPass());
      FPM->doInitialization();

      for (auto &F : M)
        FPM->run(F);
    });

    return std::move(TSM);
  }
};


#endif
