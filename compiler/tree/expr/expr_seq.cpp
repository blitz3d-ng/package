#include "expr_seq.h"
#include "const.h"
#include "../../ex.h"

/////////////////////////////
// Sequence of Expressions //
/////////////////////////////
void ExprSeqNode::semant( Environ *e ){
	for( int k=0;k<exprs.size();++k ){
		if( exprs[k] ) exprs[k]=exprs[k]->semant( e );
	}
}

TNode *ExprSeqNode::translate( Codegen *g,bool cfunc ){
	TNode *t=0,*l=0;
	for( int k=0;k<exprs.size();++k ){

		TNode *q=exprs[k]->translate(g);

		if( cfunc ){
			Type *ty=exprs[k]->sem_type;
			if( ty->stringType() ){
				q=call( "__bbStrToCStr",q );
			}else if( ty->structType() ){
				q=d_new TNode( IR_MEM,q );
			}else if( ty==Type::void_type ){
				q=d_new TNode( IR_MEM,add(q,iconst(4)) );
			}
		}

		TNode *p;
		p=d_new TNode( IR_ARG,0,0,k*4 );
		p=d_new TNode( IR_MEM,p,0 );
		p=d_new TNode( IR_MOVE,q,p );
		p=d_new TNode( IR_SEQ,p,0 );
		if( l ) l->r=p;
		else t=p;
		l=p;
	}
	return t;
}

void ExprSeqNode::castTo( DeclSeq *decls,Environ *e,bool cfunc ){
	if( exprs.size()>decls->size() ) ex( "Too many parameters" );
	for( int k=0;k<decls->size();++k ){
		Decl *d=decls->decls[k];
		if( k<exprs.size() && exprs[k] ){

			if( cfunc && d->type->structType() ){
				if( exprs[k]->sem_type->structType() ){
				}else if( exprs[k]->sem_type->intType() ){
					exprs[k]->sem_type=Type::void_type;
				}else{
					ex( "Illegal type conversion" );
				}
				continue;
			}

			exprs[k]=exprs[k]->castTo( d->type,e );

		}else{
			if( !d->defType ) ex( "Not enough parameters" );
			ExprNode *expr=constValue( d->defType );
			if( k<exprs.size() ) exprs[k]=expr;
			else exprs.push_back( expr );
		}
	}
}

void ExprSeqNode::castTo( Type *t,Environ *e ){
	for( int k=0;k<exprs.size();++k ){
		exprs[k]=exprs[k]->castTo( t,e );
	}
}
