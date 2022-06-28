#include "return.h"
#include "../expr/int_const.h"
#include "../expr/float_const.h"
#include "../expr/string_const.h"
#include "../expr/null.h"

////////////////////////////
// Return from a function //
////////////////////////////
void ReturnNode::semant( Environ *e ){
	if( e->level<=0 && expr ){
		ex( "Main program cannot return a value" );
	}
	if( e->level>0 ){
		if( !expr ){
			if( e->returnType==Type::float_type ){
				expr=d_new FloatConstNode( 0 );
			}else if( e->returnType==Type::string_type ){
				expr=d_new StringConstNode( "" );
			}else if( e->returnType->structType() ){
				expr=d_new NullNode();
			}else{
				expr=d_new IntConstNode( 0 );
			}
		}
		expr=expr->semant( e );
		expr=expr->castTo( e->returnType,e );
		returnLabel=e->funcLabel+"_leave";
	}
}

void ReturnNode::translate( Codegen *g ){
	if( !expr ){
		g->code( d_new TNode( IR_RET,0,0 ) );
		return;
	}

	TNode *t=expr->translate( g );

	if( expr->sem_type==Type::float_type ){
		g->code( d_new TNode( IR_FRETURN,t,0,returnLabel ) );
	}else{
		g->code( d_new TNode( IR_RETURN,t,0,returnLabel ) );
	}
}

#ifdef USE_LLVM
void ReturnNode::translate2( Codegen_LLVM *g ){
	auto func=g->builder->GetInsertBlock()->getParent();

	if( expr ){
		auto block=llvm::BasicBlock::Create( *g->context,"_ret",func );

		g->builder->CreateBr( block );
		g->builder->SetInsertPoint( block );

		llvm::Value *v=expr->translate2( g );

		if( expr->sem_type->structType() ){
			v=g->builder->CreateBitOrPointerCast( v,expr->sem_type->llvmType( g->context.get() ) );
		}

		g->builder->CreateRet( v );
	}else{
		auto br=g->builder->CreateIndirectBr( g->CallIntrinsic( "_bbPopGosub",llvm::PointerType::get( *g->context,0 ),0 ) );
		// TODO: a bit imprecise...should try to determine the only possible labels
		for( auto value:g->labels ){
			auto label=value.second;
			if( label->getParent()==func ){
				br->addDestination( label );
			}
		}
	}

	auto cont=llvm::BasicBlock::Create( *g->context,"ret_cont",func );
	g->builder->SetInsertPoint( cont );
}
#endif

json ReturnNode::toJSON( Environ *e ){
	json tree;tree["@class"]="ReturnNode";
	tree["pos"]=pos;
	if( expr ) tree["expr"]=expr->toJSON( e );
	tree["returnLabel"]=returnLabel;
	return tree;
}
