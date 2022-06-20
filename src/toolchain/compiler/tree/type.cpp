
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
		return llvm::ConstantFP::get( *c,llvm::APFloat(0.0) );
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

#ifdef USE_LLVM
llvm::Function *FuncType::llvmFunction( string &ident, Codegen_LLVM *g ){
	if( symbol=="" ){
		symbol="f"+ident;
	}

	llvm::Function *func = g->module->getFunction( symbol );
	if( !func ){
		vector<llvm::Type*> decls;
		for( int k=0;k<params->size();k++ ){
			decls.push_back( params->decls[k]->type->llvmType( g->context.get() ) );
		}

		auto ret_type=returnType->llvmType( g->context.get() );

		llvm::FunctionType *ft=llvm::FunctionType::get( ret_type,decls,false );

		func=llvm::Function::Create( ft,llvm::GlobalValue::ExternalLinkage,symbol,g->module.get() );
		func->setCallingConv( llvm::CallingConv::C );
	}

	return func;
}
#endif

#ifdef USE_LLVM
llvm::Type *ArrayType::llvmType( llvm::LLVMContext *c ){
	static llvm::StructType *ty=0;
	if( !ty ) {
		vector<llvm::Type*> els;
		els.push_back( llvm::PointerType::get( llvm::Type::getVoidTy( *c ),0 ) );
		ty=llvm::StructType::create( *c,els,"BBArray" );
	}
	return llvm::PointerType::get( ty,0 );
}
#endif

bool StructType::canCastTo( Type *t ){
	return t==this || t==Type::null_type || (this==Type::null_type && t->structType());
}

#ifdef USE_LLVM
llvm::Value *StructType::llvmTypeDef( Codegen_LLVM *g ){
	if( !objty ){
		deftype=llvm::StructType::create( *g->context,"_t"+ident+"data" );
		auto temp=new llvm::GlobalVariable( *g->module,deftype,false,llvm::GlobalValue::ExternalLinkage,nullptr,"_t"+ident+"type" );
		temp->setAlignment( llvm::MaybeAlign(8) );
		objty=temp;
	}
	return objty;
}

llvm::Type *StructType::llvmType( llvm::LLVMContext *c ){
	if(! structtype ){
		structtype=llvm::StructType::create( *c,"_t"+ident );
	}
	return llvm::PointerType::get( structtype,0 );
}

llvm::Constant *StructType::llvmZero( llvm::LLVMContext *c ){
	return llvm::ConstantPointerNull::get( (llvm::PointerType*)llvmType( c ) );
}
#endif


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
	if( !ty ){
		int sz=1;
		for( int k=0;k<sizes.size();++k ) sz*=sizes[k];
		ty=llvm::ArrayType::get( elementType->llvmType( c ),sz );
	}
	return llvm::PointerType::get( ty,0 );
}

llvm::GlobalVariable *VectorType::llvmDef( Codegen_LLVM *g ){
	if( !temp ){
		int sz=1;
		for( int k=0;k<sizes.size();++k ) sz*=sizes[k];

		llvm::GlobalVariable* gt=0;
		string t;
		Type *type=elementType;
		if( type==Type::int_type ) t="_bbIntType";
		else if( type==Type::float_type ) t="_bbFltType";
		else if( type==Type::string_type ) t="_bbStrType";
		else if( StructType *s=type->structType() ) gt=(llvm::GlobalVariable*)s->llvmTypeDef( g );
		else if( VectorType *v=type->vectorType() ) t=label;
		if( !gt ) {
			assert( t.size() );
			gt=(llvm::GlobalVariable*)g->module->getOrInsertGlobal( t,g->bbType );
		}

		vector<llvm::Constant*> els;
		els.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 64,6  ) ) ); // type
		els.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 64,sz ) ) ); // size
		els.push_back( gt );                                                         // elementType
		auto init=llvm::ConstantStruct::get( g->bbVecType,els );

		temp=new llvm::GlobalVariable( *g->module,g->bbVecType,false,llvm::GlobalValue::ExternalLinkage,nullptr,label );
		temp->setInitializer( init );
	}

	return temp;
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
