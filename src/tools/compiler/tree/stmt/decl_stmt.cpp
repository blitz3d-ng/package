#include "decl_stmt.h"

///////////////////
// a declaration //
///////////////////
void DeclStmtNode::semant( Environ *e ){
	decl->proto( e->decls,e );
	decl->semant( e );
}

void DeclStmtNode::translate( Codegen *g ){
	decl->translate( g );
}

#ifdef USE_LLVM
void DeclStmtNode::translate2( Codegen_LLVM *g ){
	decl->translate2( g );
}
#endif

json DeclStmtNode::toJSON( Environ *e ){
	json tree;tree["@class"]="DeclStmtNode";
	tree["pos"]=pos;
	tree["decl"]=decl->toJSON( e );
	return tree;
}
