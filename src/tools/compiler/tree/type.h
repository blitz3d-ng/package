
#ifndef TYPE_H
#define TYPE_H

#include "decl.h"

#ifdef USE_LLVM
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include "../codegen_llvm/codegen_llvm.h"
#endif

struct FuncType;
struct ArrayType;
struct StructType;
struct ConstType;
struct VectorType;

struct Type{
	virtual ~Type(){}

	virtual bool intType(){ return 0;}
	virtual bool floatType(){ return 0; }
	virtual bool stringType(){ return 0; }

	//casts to inherited types
	virtual FuncType *funcType(){ return 0; }
	virtual ArrayType *arrayType(){ return 0; }
	virtual StructType *structType(){ return 0; }
	virtual ConstType *constType(){ return 0; }
	virtual VectorType *vectorType(){ return 0; }

	//operators
	virtual bool canCastTo( Type *t ){ return this==t; }

	//built in types
	static Type *void_type,*int_type,*float_type,*string_type,*null_type;

	virtual json toJSON(){ return json(); }
	virtual json toFullJSON(){ return toJSON(); }

#ifdef USE_LLVM
	virtual llvm::Type *llvmType( llvm::LLVMContext *c )=0;
	virtual llvm::Constant *llvmZero( llvm::LLVMContext *c )=0;
#endif
};

struct FuncType : public Type{
	Type *returnType;
	DeclSeq *params;
	bool userlib,cfunc;
	std::string symbol;
	FuncType( Type *t,DeclSeq *p,bool ulib,bool cfn ):returnType(t),params(p),userlib(ulib),cfunc(cfn){}
	~FuncType(){ delete params; }
	FuncType *funcType(){ return this; }
#ifdef USE_LLVM
	llvm::Type *llvmType( llvm::LLVMContext *c );
	llvm::Constant *llvmZero( llvm::LLVMContext *c );
	llvm::Function *llvmFunction( std::string &ident, Codegen_LLVM *g );
#endif

	json toJSON(){
		json tree;tree["@class"]="FuncType";
		tree["return_type"]=returnType->toJSON();
		tree["params"]=params->toJSON();
		tree["userlib"]=userlib;
		tree["cfunc"]=cfunc;
		tree["symbol"]=symbol;
		return tree;
	}
};

struct ArrayType : public Type{
	Type *elementType;int dims;
	ArrayType( Type *t,int n ):elementType(t),dims(n){}
	ArrayType *arrayType(){ return this; }

	json toJSON(){
		json tree;tree["@class"]="ArrayType";
		tree["elementType"]=elementType->toJSON();
		tree["dims"]=dims;
		return tree;
	}

#ifdef USE_LLVM
	virtual llvm::Type *llvmType( llvm::LLVMContext *c );
	virtual llvm::Constant *llvmZero( llvm::LLVMContext *c );
#endif
};

struct StructType : public Type{
	std::string ident;
	DeclSeq *fields;
	StructType( const std::string &i ):ident(i),fields(0){}
	StructType( const std::string &i,DeclSeq *f ):ident(i),fields( f ){}
	~StructType(){ delete fields; }
	StructType *structType(){ return this; }
	virtual bool canCastTo( Type *t );

#ifdef USE_LLVM
	llvm::StructType *structtype=0;
	llvm::StructType *deftype=0;
	llvm::Value *objty=0;
	virtual llvm::Value *llvmTypeDef( Codegen_LLVM *g );

	virtual llvm::Type *llvmType( llvm::LLVMContext *c );
	virtual llvm::Constant *llvmZero( llvm::LLVMContext *c );
#endif

	json toJSON(){
		json tree;tree["@class"]="StructType";
		tree["ident"]=ident;
		return tree;
	}

	json toFullJSON(){
		json tree;tree["@class"]="StructType";
		tree["ident"]=ident;
		tree["fields"]=fields->toJSON();
		return tree;
	}
};

struct ConstType : public Type{
	Type *valueType;
	int intValue;
	float floatValue;
	std::string stringValue;
	ConstType( int n ):intValue(n),valueType(Type::int_type){}
	ConstType( float n ):floatValue(n),valueType(Type::float_type){}
	ConstType( const std::string &n ):stringValue(n),valueType(Type::string_type){}
	ConstType *constType(){ return this; }

#ifdef USE_LLVM
	llvm::Type *llvmType( llvm::LLVMContext *c );
	llvm::Constant *llvmZero( llvm::LLVMContext *c );
#endif

	json toJSON(){
		json tree;tree["@class"]="ConstType";
		tree["valueType"]=valueType->toJSON();
		tree["intValue"]=intValue;
		tree["floatValue"]=floatValue;
		tree["stringValue"]=stringValue;
		return tree;
	}
};

struct VectorType : public Type{
	std::string label;
	Type *elementType;
	std::vector<int> sizes;
	VectorType( const std::string &l,Type *t,const std::vector<int> &szs ):label(l),elementType(t),sizes(szs){}
	VectorType *vectorType(){ return this; }
	virtual bool canCastTo( Type *t );
	json toJSON();

#ifdef USE_LLVM
	llvm::ArrayType *ty=0;
	llvm::GlobalVariable *temp=0;
	virtual llvm::Type *llvmType( llvm::LLVMContext *c );
	virtual llvm::GlobalVariable *llvmDef( Codegen_LLVM *g );
	virtual llvm::Constant *llvmZero( llvm::LLVMContext *c );
#endif
};

#endif
