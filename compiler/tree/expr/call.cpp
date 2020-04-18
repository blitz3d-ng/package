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
