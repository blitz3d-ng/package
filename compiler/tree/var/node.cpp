#include "node.h"

//////////////////////////////////
// Common get/set for variables //
//////////////////////////////////
TNode *VarNode::load( Codegen *g ){
	TNode *t=translate( g );
	if( sem_type==Type::string_type ) return call( "__bbStrLoad",t );
	return mem( t );
}

TNode *VarNode::store( Codegen *g,TNode *n ){
	TNode *t=translate( g );
	if( sem_type->structType() ) return call( "__bbObjStore",t,n );
	if( sem_type==Type::string_type ) return call( "__bbStrStore",t,n );
	return move( n,mem( t ) );
}

bool VarNode::isObjParam(){
	return false;
}
