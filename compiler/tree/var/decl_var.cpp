#include "decl_var.h"

//////////////////
// Declared var //
//////////////////
void DeclVarNode::semant( Environ *e ){
}

TNode *DeclVarNode::translate( Codegen *g ){
	if( sem_decl->kind==DECL_GLOBAL ) return global( "_v"+sem_decl->name );
	return local( sem_decl->offset );
}

TNode *DeclVarNode::store( Codegen *g,TNode *n ){
	if( isObjParam() ){
		TNode *t=translate( g );
		return move( n,mem( t ) );
	}
	return VarNode::store( g,n );
}

bool DeclVarNode::isObjParam(){
	return sem_type->structType() && sem_decl->kind==DECL_PARAM;
}

json DeclVarNode::toJSON( Environ *e ){
	json tree;tree["@class"]="DeclVarNode";
	tree["sem_type"]=sem_type->toJSON();
	tree["sem_decl"]=sem_decl->toJSON();
	return tree;
}
