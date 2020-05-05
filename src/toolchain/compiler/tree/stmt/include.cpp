#include "include.h"

void IncludeNode::translate( Codegen *g ){

	if( g->debug ) g->s_data( file,label );

	stmts->translate( g );
}

#ifdef USE_LLVM
void IncludeNode::translate2( Codegen_LLVM *g ){
	stmts->translate2( g );
}
#endif

json IncludeNode::toJSON( Environ *e ){
	json tree;tree["@class"]="IncludeNode";
	tree["pos"]=pos;
	tree["file"]=file;
	tree["label"]=label;
	tree["stmts"]=stmts->toJSON( e );
	return tree;
}
