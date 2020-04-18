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
