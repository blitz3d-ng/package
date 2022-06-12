#include "call.h"

///////////////////
// Function call //
///////////////////
ExprNode *CallNode::semant( Environ *e ){
	Type *t=e->findType( tag );
	sem_decl=e->findFunc( ident );
	if( !sem_decl || !(sem_decl->kind & DECL_FUNC) ) ex( "Function '"+ident+"' not found" );
	FuncType *f=sem_decl->type->funcType();
	if( t && f->returnType!=t ) ex( "incorrect function return type" );
	exprs->semant( e );
	exprs->castTo( f->params,e,f->cfunc );
	sem_type=f->returnType;
	return this;
}

TNode *CallNode::translate( Codegen *g ){

	FuncType *f=sem_decl->type->funcType();

	TNode *t;
	TNode *l=global( "_f"+ident );
	TNode *r=exprs->translate( g,f->cfunc );

	if( f->userlib ){
		l=d_new TNode( IR_MEM,l );
		usedfuncs.insert( ident );
	}

	if( sem_type==Type::float_type ){
		t=d_new TNode( IR_FCALL,l,r,exprs->size()*4 );
	}else{
		t=d_new TNode( IR_CALL,l,r,exprs->size()*4 );
	}

	if( f->returnType->stringType() ){
		if( f->cfunc ){
			t=call( "__bbCStrToStr",t );
		}
	}
	return t;
}

#ifdef USE_LLVM
#include <llvm/IR/Attributes.h>

llvm::Value *CallNode::translate2( Codegen_LLVM *g ){
	string symbol=sem_decl->type->funcType()->symbol;

	if( symbol=="" ){
		symbol="f"+sem_decl->name;
	}

	llvm::Function *func = g->module->getFunction(symbol);
	if( !func ){
		FuncType *f=sem_decl->type->funcType();

		vector<llvm::Type*> decls;
		for( int k=0;k<f->params->size();k++ ){
			decls.push_back( f->params->decls[k]->type->llvmType( g->context.get() ) );
		}

		auto ret_type=f->returnType->llvmType( g->context.get() );

		llvm::FunctionType *ft=llvm::FunctionType::get( ret_type,decls,false );

		func=llvm::Function::Create( ft,llvm::GlobalValue::ExternalLinkage,symbol,g->module.get() );
		func->setCallingConv( llvm::CallingConv::C );
	}

	std::vector<llvm::Value*> args;
	for( int i=0;i<exprs->size();i++ ){
		args.push_back( exprs->exprs[i]->translate2( g ) );
	}

	return g->builder->CreateCall( func,args );
}
#endif

json CallNode::toJSON( Environ *e ){
	json tree;tree["@class"]="CallNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["ident"]=ident;
	tree["tag"]=tag;
	tree["sem_decl"]=sem_decl->toJSON();
	tree["exprs"]=exprs->toJSON( e );
	return tree;
}
