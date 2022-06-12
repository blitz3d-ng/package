#ifndef CODEGEN_LLVM_H
#define CODEGEN_LLVM_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <string>

class Codegen_LLVM {
public:
	bool debug;

	std::unique_ptr<llvm::LLVMContext> context;
	std::unique_ptr<llvm::Module> module;

	llvm::IRBuilder<> *builder;
	llvm::legacy::FunctionPassManager *optimizer;

	Codegen_LLVM( bool debug );

	llvm::BasicBlock *breakBlock;

	llvm::Value *CallIntrinsic( const std::string &symbol,llvm::Type *typ,int n,... );

	void optimize();
	bool verify();

	int dumpToObj( const std::string &path );
	void dumpToStderr();
};

#endif
