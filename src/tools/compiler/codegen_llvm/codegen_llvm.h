#ifndef CODEGEN_LLVM_H
#define CODEGEN_LLVM_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/Target/TargetMachine.h>
#include <map>
#include <string>
#include "../target.h"

class Codegen_LLVM {
public:
	Codegen_LLVM( bool debug );

	std::string target="native";
	bool debug;
	bool gosubUsed=false;

	std::unique_ptr<llvm::LLVMContext> context;
	std::unique_ptr<llvm::Module> module;

	llvm::IRBuilder<> *builder;

	llvm::TargetMachine *targetMachine;

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

	llvm::PointerType *bbObjPtr;

	llvm::Type *voidTy;
	llvm::Type *intTy;
	llvm::Type *stringTy;
	llvm::PointerType *voidPtr;

	llvm::Constant *constantInt( int i );
	llvm::Constant *constantFloat( double f );
	llvm::Constant *constantString( const std::string &s );

	std::map<std::string,llvm::GlobalVariable*> arrays;
	std::map<std::string,llvm::StructType*> arrayTypes;

	std::map<std::string,llvm::BasicBlock*> labels;
	std::map<std::string,llvm::Constant*> strings;

	llvm::GlobalVariable *bbData;
	std::vector<llvm::Constant*> data_values;

	llvm::BasicBlock *getLabel( std::string &ident );
	llvm::GlobalVariable *getArray( std::string &ident,int dims );

	llvm::Function *bbMain;

	void SetTarget( const Target &target );

	void optimize();
	bool verify();

	void injectMain();
	void restoreData( int count );

	int dumpToObj( std::string &out );
	void dumpToStderr();

	// debug
	llvm::DIBuilder *dbgBuilder;
	llvm::DICompileUnit *dbgCU;
	llvm::DIType *dbgFloatTy;
	std::vector<llvm::DIScope *> dbgBlocks;
};

#endif
