
#include "../std.h"
#include "type.h"

#ifdef USE_LLVM
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>

llvm::Type *Type::llvmType( llvm::LLVMContext *c ){
	return llvm::Type::getVoidTy(*c);
}

llvm::Constant *Type::llvmZero( llvm::LLVMContext *c ){
	auto pty=llvm::PointerType::get( llvm::Type::getVoidTy( *c ),0 );
	return llvm::ConstantPointerNull::get( pty );
}
#endif

static struct v_type : public Type{
	bool canCastTo( Type *t ){
		return t==Type::void_type;
	}
#ifdef USE_LLVM
	llvm::Type *llvmType( llvm::LLVMContext *c ){
		return llvm::Type::getVoidTy(*c);
	}
#endif
	json toJSON(){
		json tree;tree["@class"]="VoidType";
		return tree;
	}
}v;

static struct i_type : public Type{
	bool intType(){
		return true;
	}
	bool canCastTo( Type *t ){
		return t==Type::int_type || t==Type::float_type || t==Type::string_type;
	}
#ifdef USE_LLVM
	llvm::Type *llvmType( llvm::LLVMContext *c ){
#ifdef BB64
		return llvm::Type::getInt64Ty( *c );
#else
		return llvm::Type::getInt32Ty( *c );
#endif
	}
	llvm::Constant *llvmZero( llvm::LLVMContext *c ){
		return llvm::ConstantInt::getSigned( llvmType( c ),0 );
	}
#endif
	json toJSON(){
		json tree;tree["@class"]="IntType";
		return tree;
	}
}i;

static struct f_type : public Type{
	bool floatType(){
		return true;
	}
	bool canCastTo( Type *t ){
		return t==Type::int_type || t==Type::float_type || t==Type::string_type;
	}

#ifdef USE_LLVM
	llvm::Type *llvmType( llvm::LLVMContext *c ){
		return llvm::Type::getDoubleTy( *c );
	}
	llvm::Constant *llvmZero( llvm::LLVMContext *c ){
		return llvm::ConstantFP::get( *c,llvm::APFloat(0.0f) );
	}
#endif

	json toJSON(){
		json tree;tree["@class"]="FloatType";
		return tree;
	}
}f;

static struct s_type : public Type{
	bool stringType(){
		return true;
	}
	bool canCastTo( Type *t ){
		return t==Type::int_type || t==Type::float_type || t==Type::string_type;
	}

#ifdef USE_LLVM
	llvm::Type *llvmType( llvm::LLVMContext *c ){
		static auto ty=llvm::StructType::create( *c,"BBStr" );
		return llvm::PointerType::get( ty,0 );
	}

	llvm::Constant *llvmZero( llvm::LLVMContext *c ){
		return llvm::ConstantPointerNull::get( (llvm::PointerType*)llvmType( c ) );
	}
#endif

	json toJSON(){
		json tree;tree["@class"]="StringType";
		return tree;
	}
}s;

bool StructType::canCastTo( Type *t ){
	return t==this || t==Type::null_type || (this==Type::null_type && t->structType());
}

bool VectorType::canCastTo( Type *t ){
	if( this==t ) return true;
	if( VectorType *v=t->vectorType() ){
		if( elementType!=v->elementType ) return false;
		if( sizes.size()!=v->sizes.size() ) return false;
		for( int k=0;k<sizes.size();++k ){
			if( sizes[k]!=v->sizes[k] ) return false;
		}
		return true;
	}
	return false;
}

#ifdef USE_LLVM
llvm::Type *VectorType::llvmType( llvm::LLVMContext *c ){
	int sz=1;
	for( int k=0;k<sizes.size();++k ) sz*=sizes[k];
	return llvm::VectorType::get( elementType->llvmType( c ),sz );
}
#endif

json VectorType::toJSON(){
	json tree;tree["@class"]="VectorType";
	tree["label"]=label;
	tree["elementType"]=elementType->toJSON();
	tree["sizes"]=json::array();
	for( int i=0;i<sizes.size();i++ ){
		tree["sizes"].push_back( sizes[i] );
	}
	return tree;
}


static StructType n( "Null" );

Type *Type::void_type=&v;
Type *Type::int_type=&i;
Type *Type::float_type=&f;
Type *Type::string_type=&s;
Type *Type::null_type=&n;
