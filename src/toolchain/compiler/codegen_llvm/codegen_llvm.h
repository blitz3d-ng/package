#ifndef CODEGEN_LLVM_H
#define CODEGEN_LLVM_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <map>
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
	llvm::Value *CastToObjPtr( llvm::Value *v );
	llvm::Value *CastToArrayPtr( llvm::Value *v );

	llvm::StructType *bbType;
	llvm::StructType *bbField;
	llvm::StructType *bbObj;
	llvm::StructType *bbObjType;
	llvm::StructType *bbArray;
	llvm::StructType *bbVecType;

	std::map<std::string,llvm::BasicBlock*> labels;

	llvm::GlobalVariable *bbData;
	std::vector<llvm::Constant*> data_values;

	llvm::BasicBlock *getLabel( std::string ident );

	void optimize();
	bool verify();

	int dumpToObj( bool compileonly,const std::string &path );
	void dumpToStderr();
};

#endif
