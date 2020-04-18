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
