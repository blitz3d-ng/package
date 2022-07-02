#include "jit_orc.h"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/ExecutionEngine/JITSymbol.h>

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
	MainJD(this->ES->createBareJITDylib( "<main>" )
){
}

JIT_ORC::~JIT_ORC() {
	if( auto Err=ES->endSession() ) ES->reportError( std::move(Err) );
	if( auto Err=EPCIU->cleanup() ) ES->reportError( std::move(Err) );
}

llvm::Expected<std::unique_ptr<JIT_ORC>> JIT_ORC::Create() {
  auto EPC = llvm::orc::SelfExecutorProcessControl::Create();
  if (!EPC)
    return EPC.takeError();

  auto ES = std::make_unique<llvm::orc::ExecutionSession>(std::move(*EPC));

  auto EPCIU = llvm::orc::EPCIndirectionUtils::Create(ES->getExecutorProcessControl());
  if (!EPCIU)
    return EPCIU.takeError();

  if (auto Err = setUpInProcessLCTMReentryViaEPCIU(**EPCIU))
    return std::move(Err);

  llvm::orc::JITTargetMachineBuilder JTMB( ES->getExecutorProcessControl().getTargetTriple() );
  auto DL = JTMB.getDefaultDataLayoutForTarget();
  if( !DL ){
    return DL.takeError();
	}

  return std::make_unique<JIT_ORC>( std::move(ES),std::move(*EPCIU),std::move(JTMB),std::move(*DL) );
}

const llvm::DataLayout &JIT_ORC::getDataLayout() const { return DL; }

int JIT_ORC::run( Runtime *runtime, Codegen_LLVM *codegen, const std::string &home, const std::string &rt ) {
	auto RT = MainJD.createResourceTracker();

	// MainJD.addGenerator(cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(DL.getGlobalPrefix())));
	// MainJD.addGenerator(cantFail(llvm::orc::DynamicLibrarySearchGenerator::Load( runtime->path.c_str(),DL.getGlobalPrefix() )));

	map<const char*,void*> syms;
	runtime->loadSyms( syms );

	llvm::orc::SymbolMap symmap;
	for( auto sym:syms ){
		string ident=sym.first;
		if( !isalpha(ident[0]) && ident[0]!='_' ){
			ident=ident.substr(1);
		}
		size_t e=1;
		while( isalpha(ident[e]) || ident[e]=='_' ) e++;
		ident=ident.substr( 0,e );
		if( ident[0]!='_' ) ident="bb"+ident;

		symmap.insert(
			pair< llvm::orc::SymbolStringPtr,llvm::JITEvaluatedSymbol >(
				Mangle( ident ),
				llvm::JITEvaluatedSymbol::fromPointer( sym.second,llvm::JITSymbolFlags::Absolute|llvm::JITSymbolFlags::Exported )
			)
		);
	}

	llvm::cantFail( MainJD.define( llvm::orc::absoluteSymbols( symmap ),RT ) );

	auto TSM = llvm::orc::ThreadSafeModule( std::move(codegen->module),std::move(codegen->context) );
	cantFail( CompileLayer.add( RT,std::move(TSM) ) );

	auto start_sym = cantFail( ES->lookup({ &MainJD }, Mangle( "_bbStart" )) );
	auto main_sym  = cantFail( ES->lookup({ &MainJD }, Mangle( "bbMain" )) );
	BBSTART bbStart = (BBSTART)start_sym.getAddress();
	BBMAIN  bbMain  = (BBMAIN)main_sym.getAddress();

	char *argv[1] = { "blitzcc" };
	int retcode = bbStart( 1, argv, bbMain );

	cantFail( RT->remove() );

	return retcode;
}
