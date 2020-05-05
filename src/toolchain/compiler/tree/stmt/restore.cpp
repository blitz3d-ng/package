#include "restore.h"

//////////////////
// Restore data //
//////////////////
void RestoreNode::semant( Environ *e ){
	if( e->level>0 ) e=e->globals;

	if( ident.size()==0 ) sem_label=0;
	else{
		sem_label=e->findLabel( ident );
		if( !sem_label ) sem_label=e->insertLabel( ident,-1,pos,-1 );
	}
}

void RestoreNode::translate( Codegen *g ){
	TNode *t=global( "__DATA" );
	if( sem_label ) t=add( t,iconst( sem_label->data_sz*8 ) );
	g->code( call( "__bbRestore",t ) );
}
