#include "array_var.h"

/////////////////
// Indexed Var //
/////////////////
void ArrayVarNode::semant( Environ *e ){
	exprs->semant( e );
	exprs->castTo( Type::int_type,e );
	Type *t=e->findType( tag );
	sem_decl=e->findDecl( ident );
	if( !sem_decl || !(sem_decl->kind&DECL_ARRAY) ) ex( "Array not found" );
	ArrayType *a=sem_decl->type->arrayType();
	if( t && t!=a->elementType ) ex( "array type mismtach" );
	if( a->dims!=exprs->size() ) ex( "incorrect number of dimensions" );
	sem_type=a->elementType;
}

TNode *ArrayVarNode::translate( Codegen *g ){
	TNode *t=0;
	for( int k=0;k<exprs->size();++k ){
		TNode *e=exprs->exprs[k]->translate( g );
		if( k ){
			TNode *s=mem( add( global( "_a"+ident ),iconst( k*4+8 ) ) );
			e=add( t,mul( e,s ) );
		}
		if( g->debug ){
			TNode *s=mem( add( global( "_a"+ident ),iconst( k*4+12 ) ) );
			t=jumpge( e,s,"__bbArrayBoundsEx" );
		}else t=e;
	}
	t=add( mem( global( "_a"+ident ) ),mul( t,iconst( 4 ) ) );
	return t;
}

#ifdef USE_LLVM
llvm::Value *ArrayVarNode::translate2( Codegen_LLVM *g ){
	auto func=g->builder->GetInsertBlock()->getParent();

	auto zero=llvm::ConstantInt::get( *g->context,llvm::APInt(32, 0) );
	auto int_ty=Type::int_type->llvmType( g->context.get() );
	auto void_ty=llvm::Type::getVoidTy( *g->context );
	auto ty=sem_type->llvmType( g->context.get() );

	auto glob=g->getArray( ident,sem_decl->type->arrayType()->dims );

	vector<llvm::Value*> di;
	di.push_back( zero );
	di.push_back( zero );
	auto dataptr=g->builder->CreateLoad( g->voidPtr,g->builder->CreateGEP( g->bbArray,glob,di ) );
	auto data=g->builder->CreateBitOrPointerCast( dataptr,llvm::PointerType::get( ty,0 ) );

	auto elty=g->arrayTypes[ident];

	llvm::Value *t=0;
	for( int k=0;k<exprs->size();++k ){
		auto e=exprs->exprs[k]->translate2( g );
		if( k ){
			vector<llvm::Value*> idx;
			idx.push_back( zero );
			if( k==0 ) idx.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 32,0 ) ) );
			idx.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 32,k==0?2:k ) ) );
			auto s=g->builder->CreateLoad( int_ty,g->builder->CreateGEP( elty,glob,idx ) );
			e=g->builder->CreateAdd( t,g->builder->CreateMul( e,s ) );
		}
		if( g->debug ){
			vector<llvm::Value*> idx;
			idx.push_back( zero );
			idx.push_back( llvm::ConstantInt::get( *g->context,llvm::APInt( 32,1+k ) ) );
			auto s=g->builder->CreateLoad( int_ty,g->builder->CreateGEP( elty,glob,idx ) );

			auto ex=llvm::BasicBlock::Create( *g->context,"bounds_ex",func );
			auto cont=llvm::BasicBlock::Create( *g->context,"bounds_cont",func );

			g->builder->CreateCondBr( g->builder->CreateICmpSGE( e,s ),ex,cont );

			g->builder->SetInsertPoint( ex );
			g->CallIntrinsic( "_bbArrayBoundsEx",void_ty,0 );
			g->builder->CreateUnreachable();

			g->builder->SetInsertPoint( cont );
		}
		t=e;
	}

	vector<llvm::Value*> idx;
	idx.push_back( t );
	auto el=g->builder->CreateGEP( ty,data,idx );
	return el;
}
#endif

json ArrayVarNode::toJSON( Environ *e ){
	json tree;tree["@class"]="ArrayVarNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["ident"]=ident;
	tree["tag"]=tag;
	tree["exprs"]=exprs->toJSON( e );
	tree["sem_decl"]=sem_decl->toJSON();
	return tree;
}
