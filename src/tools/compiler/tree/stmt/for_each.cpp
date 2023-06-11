#include "for_each.h"

///////////////////////////////
// For each object of a type //
///////////////////////////////
void ForEachNode::semant( Environ *e ){
	var->semant( e );
	Type *ty=var->sem_type;

	if( ty->structType()==0 ) ex( "Index variable is not a NewType" );
	Type *t=e->findType( typeIdent );
	if( !t ) ex( "Type name not found" );
	if( t!=ty ) ex( "Type mismatch" );

	std::string brk=e->setBreak( sem_brk=genLabel() );
	stmts->semant( e );
	e->setBreak( brk );
}

void ForEachNode::translate( Codegen *g ){
	TNode *t,*l,*r;
	std::string _loop=genLabel();

	std::string objFirst,objNext;

	if( var->isObjParam() ){
		objFirst="__bbObjEachFirst2";
		objNext="__bbObjEachNext2";
	}else{
		objFirst="__bbObjEachFirst";
		objNext="__bbObjEachNext";
	}

	l=var->translate( g );
	r=global( "_t"+typeIdent );
	t=jumpf( call( objFirst,l,r ),sem_brk );
	g->code( t );

	g->label( _loop );
	stmts->translate( g );

	debug( nextPos,g );
	t=jumpt( call( objNext,var->translate( g ) ),_loop );
	g->code( t );

	g->label( sem_brk );
}

#ifdef USE_LLVM
void ForEachNode::translate2( Codegen_LLVM *g ){
	llvm::Value *t,*l,*r;
	std::string objFirst,objNext;

	if( var->isObjParam() ){
		objFirst="_bbObjEachFirst2";
		objNext="_bbObjEachNext2";
	}else{
		objFirst="_bbObjEachFirst";
		objNext="_bbObjEachNext";
	}

	auto func=g->builder->GetInsertBlock()->getParent();
	auto loop=llvm::BasicBlock::Create( *g->context,"foreach_loop",func );
	auto cont=llvm::BasicBlock::Create( *g->context,"foreach_cont",func );

	auto ity=llvm::Type::getInt64Ty( *g->context );
	auto bty=llvm::Type::getInt1Ty( *g->context );

	l=g->builder->CreateBitOrPointerCast( var->translate2( g ),g->bbObjPtr );
	r=g->builder->CreateBitOrPointerCast( var->sem_type->structType()->objty,llvm::PointerType::get( g->bbType,0 ) );

	g->builder->CreateCondBr( g->builder->CreateTruncOrBitCast( g->CallIntrinsic( objFirst,ity,2,l,r ),bty ),loop,cont );

	g->builder->SetInsertPoint( loop );

	auto oldBreakBlock=g->breakBlock;
	g->breakBlock=cont;
	stmts->translate2( g );
	g->breakBlock=oldBreakBlock;

	// debug( nextPos,g );
	g->builder->CreateCondBr( g->builder->CreateTruncOrBitCast( g->CallIntrinsic( objNext,ity,1,l ),bty ),loop,cont );

	g->builder->SetInsertPoint( cont );
}
#endif
