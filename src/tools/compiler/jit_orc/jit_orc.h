#ifndef JIT_H
#define JIT_H

#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/EPCIndirectionUtils.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/IRTransformLayer.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>

#include "../libs.h"

typedef void (*BBMAIN)();
typedef int (*BBSTART)( int,char**,BBMAIN );

class JIT_ORC {
private:
	std::unique_ptr<llvm::orc::ExecutionSession> ES;
	std::unique_ptr<llvm::orc::EPCIndirectionUtils> EPCIU;

	llvm::DataLayout DL;

	llvm::orc::MangleAndInterner Mangle;
	llvm::orc::RTDyldObjectLinkingLayer ObjectLayer;

	llvm::orc::JITDylib &MainJD;
public:
	JIT_ORC( std::unique_ptr<llvm::orc::ExecutionSession> ES,std::unique_ptr<llvm::orc::EPCIndirectionUtils> EPCIU,llvm::orc::JITTargetMachineBuilder JTMB, llvm::DataLayout DL );

	~JIT_ORC();

	static llvm::Expected<std::unique_ptr<JIT_ORC>> Create();

	int run( Runtime *runtime,const std::string &obj,const std::string &home,const std::string &rt );
};


#endif
