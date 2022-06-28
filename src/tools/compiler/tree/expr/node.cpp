#include "cast.h"

//////////////////////////////////
// Cast an expression to a type //
//////////////////////////////////
ExprNode *ExprNode::castTo( Type *ty,Environ *e ){
	if( !sem_type->canCastTo( ty ) ){
		ex( "Illegal type conversion" );
	}

	ExprNode *cast=d_new CastNode( this,ty );
	return cast->semant( e );
}

#ifdef USE_LLVM
llvm::Value *ExprNode::translate2( Codegen_LLVM *g ){
	cerr<<"translate2 missing implementation for "<<typeid(*this).name()<<endl;
	abort();
}
#endif
